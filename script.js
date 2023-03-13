const fs = require("fs")

prettyplotDatapath = "prettyplotData"

async function readFiles(path,json_folder) {
    let obj = []
    const dirs = await fs.promises.readdir(path);
    for (const dir of dirs) {
        const dirpath = path + '/' + dir
        const stat = await fs.promises.stat(path + "/" + dir);
        if(stat.isDirectory()){
            const files = await fs.promises.readdir(dirpath);
            let testArray = []
            for(const file in files){
                const data = await fs.promises.readFile(dirpath + '/' +files[file],"utf-8")
                let test = {test_type: files[file],data :data}
                testArray.push(test)
            }
            obj.push({name:dir,tests:testArray})
        }
    }

    try {
        await fs.promises.mkdir(json_folder);
    } catch (err) {
        if (err.code !== 'EEXIST') {
            throw err;
        }
    }
    await fs.promises.writeFile(json_folder + `/stats.json`, JSON.stringify(obj));
    console.log(`File object written to ${json_folder}/stats.json`);
}


module.exports  = readFiles


