import Lineplot from "../components/Lineplot"
import { useState,useEffect } from "react"
import Scatterplot from "../components/Scatterplot"
import "./Dataview.css"


function Dataview(props){
    const data = props.data
    const name = data.name
    const testsarray = data.tests


    const [test,setTest] = useState('ADD.json')
    const [type,set_type] = useState(0)
    const [plot_type,set_plot_type] = useState(0)
    const [x_data,set_x_data] = useState([])
    const [y_data,set_y_data] = useState([])

    useEffect(()=>{
        getType()
        getPlotType()
        getRightTest()
      },[test,type,plot_type])

    const getPlotType = ()=>{
        if(name.includes("Avg_")) set_plot_type(0)
        else set_plot_type(1)
    }

    const getType = () =>{
        if(test.includes("T_")) set_type(0)
        else set_type(1)
    }

    const getRightTest = () =>{
        let temp_x_array = []
        let temp_y_array = []

        for(let i in testsarray){
        if(testsarray[i].test_type === test && type === 1){

            let data = JSON.parse(testsarray[i].data)

            let times = data.Time
            let Threads = data.Threads

            for(let time in times){
                temp_y_array.push(times[time])
            }
            for(let thread in Threads){
                temp_x_array.push(Threads[thread])
            }
        }

        else if(testsarray[i].test_type === test && type === 0){

            let data = JSON.parse(testsarray[i].data)

            let actions = data.Actions
            let Threads = data.Threads

            for(let action in actions){
                temp_y_array.push(actions[action])
            }
            for(let thread in Threads){
                temp_x_array.push(Threads[thread])
            }
        }
        set_x_data(temp_x_array)
        set_y_data(temp_y_array)
        }

    }

     

    const buttonTests = testsarray.map((element) =>{
        return(
        <button onClick={()=>setTest(element.test_type)} key={element.test_type}>{element.test_type}</button>
        )
    })

    return(
        <div className="plotPage">
            <h1>{name}</h1>
            <div className="plot-cont">
                { plot_type ? 
                <Scatterplot x_data={x_data} y_data={y_data} name={name} key={test}/>
                :
                <Lineplot x_data={x_data} y_data={y_data} name={name} key={test}/>
                }
            </div>
            {buttonTests}
        </div>
    )
}


export default Dataview