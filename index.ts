//let { createClient } = require("oicq");
import { createClient } from "oicq";
let ref = require("ref-napi");
let ffi = require("ffi-napi");
import fs from "fs";
import path from "path";

import { load_config } from "./configure";

import controller from "./invite_controller"

console.log("started");

let i64 = ref.types.int64
let cstr = ref.types.CString

let app_dir: string = path.join(__dirname, "./dicebot_db/");

let dicebot = ffi.Library('./dicebot.dll', {
    'func_enable': ['void', [cstr]],
    'func_disable': ['void', []],
    'func_private_message': ['void', [i64, cstr, cstr]],
    'func_group_message': ['void', [i64, i64, cstr, cstr]],
    'func_discuss_message': ['void', [i64, i64, cstr, cstr]],
    'func_set_send_private_message': ['void', ['pointer']],
    'func_set_send_group_message': ['void', ['pointer']],
    'func_set_send_discuss_message': ['void', ['pointer']],
    'func_set_log_info': ['void', ['pointer']],
    'func_set_log_warn': ['void', ['pointer']]
});

load_config().then((config) => {

    const bot = createClient(config.user_id, config.general);

    let pmsg_function = ffi.Callback('void', [i64, cstr], (uid: number, msg: string) => { bot.sendPrivateMsg(uid, msg); });
    let gmsg_function = ffi.Callback('void', [i64, cstr], (gid: number, msg: string) => {
        controller.increase_use(gid);
        bot.sendGroupMsg(gid, msg);
    });
    let dmsg_function = ffi.Callback('void', [i64, cstr], (did: number, msg: string) => { bot.sendDiscussMsg(did, msg); });
    let log_info_function = ffi.Callback('void', [cstr], (msg: string) => { bot.logger.info(msg); });
    let log_warn_function = ffi.Callback('void', [cstr], (msg: string) => { bot.logger.warn(msg); });

    process.on('exit', () => {
        pmsg_function;
        gmsg_function;
        dmsg_function;
        log_info_function;
        log_warn_function;
    })

    dicebot['func_set_send_private_message'](pmsg_function);
    dicebot['func_set_send_group_message'](gmsg_function);
    dicebot['func_set_send_discuss_message'](dmsg_function);
    dicebot['func_set_log_info'](log_info_function);
    dicebot['func_set_log_warn'](log_warn_function);

    dicebot['func_enable'](app_dir);

    controller.set_bot(bot);
    controller.set_master_id(config.master_id);
    controller.set_profile(app_dir);

    bot.on("system.login.slider", (data) => {
        process.stdin.once("data", (input) => {
            bot.submitSlider(input.toString());
        });
    });

    bot.on("system.login.device", () => {
        process.stdin.once("data", () => {
            bot.login();
        });
    });

    bot.on("message.private", (data) => {
        dicebot['func_private_message'](data.sender.user_id, data.sender.nickname, data.raw_message);

        if (data.sender.user_id == config.master_id)
            controller.message_process(data.raw_message);
    });

    bot.on("message.group", (data) => {
        dicebot['func_group_message'](data.group_id, data.sender.user_id, data.sender.nickname, data.raw_message);
        controller.increase_burden(data.group_id);
    });

    bot.on("message.discuss", (data) => {
        dicebot['func_discuss_message'](data.discuss_id, data.sender.user_id, data.sender.nickname, data.raw_message);
    });

    bot.on("request.friend", (data) => {
        if (controller.check_friend_req(data.comment)) {
            bot.setFriendAddRequest(data.flag);
            bot.sendPrivateMsg(config.master_id, `收到好友请求\nQQ：${data.nickname}(${data.user_id})\n请求文本：${data.comment}`);
        }
    });

    bot.on("request.group.invite", (data) => {
        bot.setGroupAddRequest(data.flag);
        bot.getStrangerInfo(data.user_id).then((uinfo) => {
            bot.getGroupInfo(data.group_id).then((ginfo) => {
                bot.sendPrivateMsg(config.master_id,
                    `收到加群邀请\nQQ：${uinfo.nickname}(${data.user_id})\n群：${ginfo.group_name}(${data.group_id})`);
            })
        })
    });

    bot.on("notice.group.increase", (data) => {
        bot.getGroupInfo(data.group_id).then((ginfo) => {
            if (!controller.check_group_info(ginfo)) {
                bot.setGroupLeave(ginfo.group_id);
                bot.sendPrivateMsg(config.master_id, `退出群：${ginfo.group_name}(${ginfo.group_id})`)
            }
        });
    });

    bot.login(config.password);
});

