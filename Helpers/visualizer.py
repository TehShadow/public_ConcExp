import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os
from pprint import pprint as pp

class Plotter:
    def __init__(self):
        self.plot_path = "./Logs/Plots/"
        self.log_path = "./Logs/Data/"
        self.index_labels = ["Threads","Time","Actions","ThreadPinning","LocalWorkTimer","LocalWork"]
        self.color = ['b','g','r','c','m','y','k','w']
        self.line = ['-','--','-.',':','.','o','*','x']
        self.DataArray = []
        self.pretty_path = "./Logs/Data/prettyplotData/"
        self.create_dir(self.log_path,"prettyplotData")

    def createDictionary(self,name,location,type,test,pinning):
        data = {
            "name" : name,
            "location": location,
            "type" : type,
            "testType" : test,
            "nopinning" : pinning,
            "avgLocation" : "" 
        }
        return data

    def getData(self):
        # info for dictionary
        name = ""
        location = ""
        type = ""
        test = ""
        pinning = False

        ####################################

        path , dirs , files = next(os.walk(self.log_path))
        for dir in dirs:
            if(dir == "prettyplotData"): continue
            name = dir
            # self.create_dir(self.plot_path,dir)
            path_2 , dirs_2 , files_2 = next(os.walk(self.log_path+dir))
            for file in files_2:
                location = self.log_path+dir+"/"+file
                # load data
                dataNp = np.loadtxt(location,dtype=float,delimiter=":")
                data = pd.DataFrame(dataNp,columns=self.index_labels)
                # type
                if data["Time"][0] == data["Time"][1] and data["Time"][1] == data["Time"][2]: type = "Actions"
                else : type = "Time"
                # test
                test = location.split("/")[4].replace(".txt","")
                # pinning
                pinning = location.split("/")[4].__contains__("no")

                self.DataArray.append(self.createDictionary(name,location,type,test,pinning))

    def Plot(self):
        for x in self.DataArray:
            if x["type"] == "Actions": self.ActionPlotter(x["location"],x["name"],x["testType"])
            else: self.TimePlotter(x["location"],x["name"],x["testType"])

    def ActionPlotter(self,location,name,testType):
        # loads data
        dataNp = np.loadtxt(location,dtype=float,delimiter=":")
        data = pd.DataFrame(dataNp,columns=self.index_labels)

        #plots data
        plt.figure(figsize=(10,7))
        plt.rcParams["figure.autolayout"]
        if name.__contains__("Avg"): plt.plot(data["Threads"] , data["Actions"]*0.00001/1000 , label=testType)
        else: plt.scatter(x=data["Threads"] , y=data["Actions"]*0.00001/1000 , label=testType)
        plt.xlabel("Threads")
        plt.ylabel("Throughput (MilActions/sec)")
        plt.title(name)
        plt.legend()
        self.create_dir(self.plot_path , name)
        self.create_dir(self.pretty_path , name)
        data.to_json(self.pretty_path + name + "/" + testType + ".json")
        plt.savefig(self.plot_path + name + "/" + testType + ".png")
        plt.close()

    def TimePlotter(self,location,name,testType):
        # loads data
        dataNp = np.loadtxt(location,dtype=float,delimiter=":")
        data = pd.DataFrame(dataNp,columns=self.index_labels)

        #plots data
        plt.figure(figsize=(10,7))
        plt.rcParams["figure.autolayout"]
        if name.__contains__("Avg"): plt.plot(data["Threads"] , data["Time"], label=testType)
        else: plt.scatter(x=data["Threads"] , y=data["Time"], label=testType)
        plt.xlabel("Threads")
        plt.ylabel("Time (ms)")
        plt.title(name)
        plt.legend()
        self.create_dir(self.plot_path , name)
        self.create_dir(self.pretty_path , name)
        data.to_json(self.pretty_path + name + "/" + testType + ".json")
        plt.savefig(self.plot_path + name + "/" + testType + ".png")
        plt.close()

    def AvgData(self):
        for x in range(len(self.DataArray)):
            if self.DataArray[x]["name"].__contains__("Avg"): continue
            
            self.create_dir(self.log_path , "Avg_" + self.DataArray[x]["name"])
            dataNp = np.loadtxt(self.DataArray[x]["location"],dtype=float,delimiter=":")
            data = pd.DataFrame(dataNp,columns=self.index_labels)
            avgdata = data.groupby("Threads")
            avgdata.mean().to_csv((self.log_path + "Avg_" + self.DataArray[x]["name"] + "/" +self.DataArray[x]["testType"]+".txt") , header=None , index=True , sep=":")
            self.DataArray[x]["avgLocation"] = self.log_path + "Avg_" + self.DataArray[x]["name"] + "/" +self.DataArray[x]["testType"]+".txt"


    def plotAllAvgHelper(self,array):
        plt.figure(figsize=(10,7))
        plt.rcParams["figure.autolayout"]
        plt.title(array[0]["testType"])
        if array[0]["type"] == "Time":
            plt.xlabel("Threads")
            plt.ylabel("Time (ms)")
            for x in array:
                dataNp = np.loadtxt(x["avgLocation"],dtype=float,delimiter=":")
                data = pd.DataFrame(dataNp,columns=self.index_labels)
                plt.plot(data["Threads"] , data["Time"], label=x["name"])
        else:
            plt.xlabel("Threads")
            plt.ylabel("Throughput (MilActions/sec)")
            for y in array:
                dataNp = np.loadtxt(y["avgLocation"],dtype=float,delimiter=":")
                data = pd.DataFrame(dataNp,columns=self.index_labels)
                plt.plot(data["Threads"] , data["Actions"]*0.00001/1000 , label=y["name"])
        
        plt.legend()
        self.create_dir(self.plot_path,"All")
        plt.savefig(self.plot_path + "All" + "/" + array[0]["testType"]+".png")


    def plotAllAvg(self):
        sortTestTypes = []
        pathsTestTypes = []
        for x in self.DataArray:
            if not sortTestTypes.__contains__(x["testType"]): sortTestTypes.append(x["testType"])
        

        for y in sortTestTypes:
            for x in self.DataArray:
                if x["name"].__contains__("Avg"): continue
                if y == x["testType"]: pathsTestTypes.append(x)

            self.plotAllAvgHelper(pathsTestTypes)
            pathsTestTypes.clear()



    def create_dir(self, path ,dirName):
        if os.path.exists(path + dirName):
            return
        else:
            os.mkdir(path + dirName)



if __name__ == "__main__":
    log = Plotter()
    log.getData()
    log.AvgData()
    log.Plot()
    log.plotAllAvg()
