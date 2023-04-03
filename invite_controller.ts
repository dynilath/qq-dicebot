import { Client, GroupInfo } from "icqq";

import fs from "fs";
import path from "path";

let master_id: number;
let profile_name: string = "burden_conunt.json";

function set_master_id(id: number) {
    master_id = id;
}

interface burden_item {
    time: number,
    usage_count: number,
    total_count: number
}

function fork(criteria: boolean, good: Function, bad: Function) {
    if (criteria) good();
    else bad();
}

const max_time = 60;

let bot: Client | undefined;

function set_bot(nbot: Client) {
    bot = nbot;
}

let burden_list: burden_item[] = [];
let burden_counter: Map<number, { usage_count: number, total_count: number }> = new Map<number, { usage_count: number, total_count: number }>();

function what_minute_now() {
    return Math.floor(new Date().getTime() / 60000)
}

function trim_burden(list: burden_item[], now: number) {
    if (list.length == 0 || list[0].time + max_time > now) return;
    list.shift();
    trim_burden(list, now);
}

function adjust_burden(list: burden_item[], now: number, is_cmd: boolean) {
    trim_burden(list, now);
    let usage_inc = is_cmd ? 1 : 0;
    if (list.length == 0 || list[list.length - 1].time != now)
        list.push({ time: now, usage_count: usage_inc, total_count: 1 });
    else {
        list[list.length - 1].total_count += 1;
        list[list.length - 1].usage_count += usage_inc;
    }
}

function adjust_burden_map(gid: number, is_cmd: boolean) {
    if (!burden_counter.has(gid)) burden_counter.set(gid, { usage_count: 0, total_count: 0 });

    let target = burden_counter.get(gid);
    if (typeof target != 'undefined') {
        if (is_cmd) target.usage_count += 1;
        else target.total_count += 1;
    }

    adjust_burden(burden_list, what_minute_now(), is_cmd);
}

async function set_profile(prf_dir: string) {
    let path_to_prf: string = path.join(prf_dir, profile_name);
    if (fs.existsSync(path_to_prf)) {
        let data = JSON.parse(fs.readFileSync(path_to_prf, { encoding: 'utf-8' }));
        burden_counter.clear()
        for (let k in data.burden_counter) {
            burden_counter.set(Number.parseInt(k), data.burden_counter[k]);
        }
        burden_list = data.burden_list;
    }

    setInterval(() => {
        let burden_counter_cp: any = {};
        burden_counter.forEach((v, k) => {
            burden_counter_cp[k] = v;
        })

        fs.writeFileSync(path_to_prf, JSON.stringify({ "burden_counter": burden_counter_cp, "burden_list": burden_list }), { encoding: 'utf-8' });
        bot?.logger?.debug("Burden data saved.")
    }, 10000);
}

async function top_usage(tail: string) {
    let sort_arr: { id: number, usage_count: number, total_count: number }[] = [];
    burden_counter.forEach((v, k) => sort_arr.push({ id: k, usage_count: v.usage_count, total_count: v.total_count }));
    sort_arr.sort((a, b) => {
        if (a.usage_count == b.usage_count) return b.total_count - a.total_count;
        else return a.usage_count - b.usage_count;
    });

    trim_burden(burden_list, what_minute_now());
    let global_burden = burden_list.reduce((pV, cV) => {
        pV.total_count += cV.total_count;
        pV.usage_count += cV.usage_count;
        return pV
    },
        { total_count: 0, usage_count: 0 });

    let msg = `${max_time}分钟内指令数/消息总数: ${global_burden.usage_count}/${global_burden.total_count}\n群消息统计如下：`
    for (let i = 0; i < 10 && i < sort_arr.length; i++) {
        if (bot != null) {
            let ginfo = await bot.getGroupInfo(sort_arr[i].id);
            msg = msg.concat(`\n${i + 1}：群 ${ginfo.group_name}(${ginfo.group_id}) : ${sort_arr[i].usage_count}/${sort_arr[i].total_count}`);
        }
    }
    bot?.sendPrivateMsg(master_id, msg);
}

async function top_clear(tail: string) {
    burden_counter.forEach((v, k) => { v.total_count = v.usage_count = 0; });
    bot?.sendPrivateMsg(master_id, "已重置所有群消息统计");
}

async function top_general(tail: string) {
    let sort_arr: { id: number, usage_count: number, total_count: number }[] = [];
    burden_counter.forEach((v, k) => sort_arr.push({ id: k, usage_count: v.usage_count, total_count: v.total_count }));
    sort_arr.sort((a, b) => {
        if (a.total_count == b.total_count) return a.usage_count - b.usage_count;
        return b.total_count - a.total_count;
    });

    trim_burden(burden_list, what_minute_now());
    let global_burden = burden_list.reduce((pV, cV) => {
        pV.total_count += cV.total_count;
        pV.usage_count += cV.usage_count;
        return pV
    },
        { total_count: 0, usage_count: 0 });

    let msg = `${max_time}分钟内指令数/消息总数: ${global_burden.usage_count}/${global_burden.total_count}\n群消息统计如下：`
    for (let i = 0; i < 10 && i < sort_arr.length; i++) {
        if (bot != null) {
            let ginfo = await bot.getGroupInfo(sort_arr[i].id);
            msg = msg.concat(`\n${i + 1}：群 ${ginfo.group_name}(${ginfo.group_id}) : ${sort_arr[i].usage_count}/${sort_arr[i].total_count}`);
        }
    }
    bot?.sendPrivateMsg(master_id, msg);
}

function a_number_then(src: string, call: Function) {
    let sn = src.match('\\d+');
    if (sn != null && sn.length > 0) call(Number.parseInt(sn[0]));
}

function group_info(tail: string) {
    a_number_then(tail, (gid: number) =>
        bot?.getGroupInfo(gid).then((ginfo) => {
            let tv = burden_counter.get(gid);
            bot?.sendPrivateMsg(master_id, `群：${ginfo.group_name} (${ginfo.group_id})\n`
                + `成员数：${ginfo.member_count}\n`
                + `指令使用/总负载：${tv?.usage_count}/${tv?.total_count}`)
        })
    );
}

function group_exit(tail: string) {
    a_number_then(tail, (gid: number) =>
        bot?.getGroupInfo(gid).then((ginfo) => {
            bot?.setGroupLeave(gid).then((retc) => {
                bot?.sendPrivateMsg(master_id, `"退出群：${ginfo.group_name} (${ginfo.group_id})`);
            })
        })
    );
}

function user_info(tail: string) {
    a_number_then(tail, (uid: number) =>
        bot?.getStrangerInfo(uid).then((uinfo) => {
            bot?.sendPrivateMsg(master_id, `用户：${uinfo.nickname}(${uinfo.user_id})`);
        }
        )
    );
}

function repeat(tail: string) {
    bot?.sendPrivateMsg(master_id, tail);
}

let dispatcher = [
    { name: "group info", func: group_info },
    { name: "group exit", func: group_exit },
    { name: "user info", func: user_info },
    { name: "repeat", func: repeat },
    { name: "top general", func: top_general },
    { name: "top clear", func: top_clear },
    { name: "top", func: top_usage }
]

function message_process(msg: string) {
    let trimmed = msg.trimLeft();
    let f = dispatcher.find((v) => trimmed.startsWith(v.name));
    if (typeof f != 'undefined')
        f.func(trimmed.substr(trimmed.indexOf(f.name) + f.name.length));
}

function increase_use(gid: number) {
    adjust_burden_map(gid, true);
}

function increase_burden(gid: number) {
    adjust_burden_map(gid, false);
}

let bad_grp_names = ["红包", "互赞", "名片赞", "聊天", "邀人", "免费", "拉人", "表白", "交友", "拉一个人", "拉一人"];
let bad_frd_propos = ["扩列", "互暖", "互z", "拉我", "互赞"];

function check_group_info(data: GroupInfo) {
    for (let v in bad_grp_names) {
        if (data.group_name.indexOf(bad_grp_names[v]) > 0) return false;
    }
    return true;
}

function check_friend_req(cmt: string) {
    for (let v in bad_frd_propos) {
        if (cmt.indexOf(v) > 0) return false;
    }
    return true;
}

export default {
    set_bot,
    set_master_id,
    increase_use,
    increase_burden,
    message_process,
    check_friend_req,
    check_group_info,
    set_profile
};