const path = require('path')
const express = require("express")
const morgan = require("morgan")
const readfiles = require("./script")



const app = express()
const listening_port = 4000
prettyplotDatapath = "./Logs/Data/prettyplotData"
clientPath = "./client/public"


app.disable('x-powered-by')
app.use(morgan("dev"))
app.use(express.json())
app.use(express.urlencoded({extended:true}))


app.use((req,res,next)=>{
    res.header('Access-Control-Allow-Origin','*');
    res.header('Access-Control-Allow-Headers','Oring, X-Requested-With, Content-Type, Accept, Authorization')
    if(req.method === "OPTIONS"){
        res.header('Access-Control-Allow-Methods',"PUT, POST, PATCH, DELETE, GET")
        return res.status(200).json({});
    }
    next();
})

readfiles(prettyplotDatapath,clientPath)



app.use(express.static('client/build'))


app.get('*',(req,res) =>{
    res.sendFile(path.resolve(__dirname,'client','build','index.html'))
})


app.use((req,res,next)=>{
    const error = new Error("Not Found")
    error.status = 404
    next(error)
})

app.use((error,req,res,next)=>{
    res.status(error.status||500)
    res.json({
        error:{
            message : error.message
        }
    })
})

app.listen(listening_port)
console.log(`Server listening on port ${listening_port}`)


