import {Line} from "react-chartjs-2"
import {
    Chart as ChartJS,
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend,
  } from 'chart.js';
import uuid from 'react-uuid';

ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend
  );

function Lineplot(props){
    const x = props.x_data
    const y = props.y_data
    const name = props.name


    const data = {
        labels: x,
        datasets: [
            {
              label: name,
              backgroundColor : 'rgba(104, 105, 99,1)',
              borderColor: 'rgba(219, 84, 97,1)',
              data:y
            } 
          ]
        };


    const options = {
      
    }

    return(
        <div className="chart-container" style={{
                                                display:"flex",
                                                alignItems:"center",
                                                justifyContent:"center",
                                                position: "relative", 
                                                height:"60vh",
                                                width:"100vw"}}>
            <Line data={data} id={uuid()} className="plots"/>
        </div>
    )
}


export default Lineplot