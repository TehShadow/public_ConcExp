import {Scatter} from "react-chartjs-2"
import {
    Chart as ChartJS,
    LinearScale,
    PointElement,
    LineElement,
    Tooltip,
    Legend,
  } from 'chart.js';
import uuid from 'react-uuid';

ChartJS.register(LinearScale, PointElement, LineElement, Tooltip, Legend);

function Scatterplot(props){
    const x = props.x_data
    const y = props.y_data
    const name = props.name




    const data = {
        labels: x,
        datasets: [
            {
              label: name,
              fill: false,
              borderColor: "#c45850",
              data:y
            } 
          ]
        };

    return(
        <div className="chart-container" style={{
                                                display:"flex",
                                                alignItems:"center",
                                                justifyContent:"center",
                                                position: "relative", 
                                                height:"60vh",
                                                width:"100vw"}}>
            <Scatter data={data} id={uuid()} className="plots"/>
        </div>
    )
}


export default Scatterplot