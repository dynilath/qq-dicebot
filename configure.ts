import fs from "fs";
import path from "path";

const config_path = path.resolve("./config.json");

const delay = (ms: number) => new Promise(resolve => setTimeout(resolve, ms))

async function validate(src: any) {
    let do_write = false;

    let result = {
        user_id: 0,
        master_id: 0,
        password: "",
        general: {
            platform: 1,
            ignore_self: true,
            resend: true,
            kickoff: true,
        }
    }

    while (typeof src['user_id'] !== 'number') {
        do_write = true;
        console.log('Input user_id:');
        let hasinput = false;
        process.stdin.once('data', (input) => {
            src['user_id'] = Number.parseInt(input.toString());
            hasinput = true;
        })
        while (!hasinput) await delay(100);
    }
    result.user_id = src['user_id'];

    while (typeof src['password'] !== 'string' || (src['password'] as string).length < 6) {
        do_write = true;
        console.log('Input password:');
        let hasinput = false;
        process.stdin.once('data', (input) => {
            src['password'] = input.toString().trim();
            hasinput = true;
        })
        while (!hasinput) await delay(100);
    }
    result.password = src['password'];

    while (typeof src['master_id'] !== 'number') {
        do_write = true;
        console.log('Input master_id:');
        let hasinput = false;
        process.stdin.once('data', (input) => {
            src['master_id'] = Number.parseInt(input.toString());
            hasinput = true;
        });
        while (!hasinput) await delay(100);
    }
    result.master_id = src['master_id'];

    if (typeof src['general'] !== 'object'
        || typeof src['general']['platform'] !== 'number'
        || typeof src['general']['ignore_self'] !== 'boolean'
        || typeof src['general']['resend'] !== 'boolean'
        || typeof src['general']['kickoff'] !== 'boolean') {
        do_write = true;
    } else {
        result.general.platform = src.general.platform;
        result.general.ignore_self = src.general.ignore_self;
        result.general.resend = src.general.resend;
        result.general.kickoff = src.general.kickoff;
    }

    if (do_write) {
        fs.accessSync(config_path, fs.constants.W_OK);
        fs.writeFileSync(config_path, JSON.stringify(result), { encoding: "utf-8" });
    }

    return result;
}


export function load_config() {
    let buffer = "{}";
    try {
        fs.accessSync(config_path, fs.constants.R_OK);
        buffer = fs.readFileSync(config_path, { encoding: "utf-8" });
    } catch (error) {
        buffer = "{}"
    }

    let result = JSON.parse(buffer);

    return validate(result);
}