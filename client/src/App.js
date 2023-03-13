import './App.css';
import React, { useState, useEffect } from 'react';
import { BrowserRouter as Router , Route , Routes } from 'react-router-dom'
import axios from 'axios';
import Landing from './Pages/Landing';
import Dataview from './Pages/Dataview';



function App() {


  const getData =()=>{
    axios
      .get("./stats.json")
      .then((res)=>{
        const data = res.data;
        setData(data)
        // console.log(data);
      })
      .catch((er)=>{
        console.log(er)
      })
  }

  const [data,setData] = useState([])

  useEffect(()=>{
    getData()
  },[])


const listItems = data.map((element) => {
  return (
      <Route exact path={element.name} element={<Dataview data={element}/>} key={element.name}/>
  )
})


  return (
    <div className="App">
      <Router>
        <Routes>
          <Route exact path="/" element={<Landing data={data}/>}/>
          {listItems}
        </Routes>
      </Router>
    </div>
  );
}

export default App;
