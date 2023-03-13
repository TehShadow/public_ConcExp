import "./Landing.css"


function Landing(props){
    const data = props.data;

    const listItems = data.map((element) => {
        let link = "/" + element.name
        return (
            <a className="ds-item" href={link}><span className="ds-name">{element.name}</span></a>
        )
      })
    return (
        <div>
            <h1>Plot Dashboard</h1>
            <div className="ds-container">
                {listItems}
            </div>
        </div>
    )
}


export default Landing