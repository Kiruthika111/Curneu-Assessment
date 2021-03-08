import { StatusBar } from 'expo-status-bar';
import React, {Component} from 'react';
import { StyleSheet, Text, View, Alert, Button, ActivityIndicator,ScrollView } from 'react-native';
//import Header from "./Components/Header"
import axios from 'axios';
var a=1;
export default class App extends React.Component  {

  

  constructor(props){
    super(props);
    this.state = {
      isLoading: true,
      dataSource: null,
    }
  }


  componentDidMount() {
   // return fetch('https://jsonplaceholder.typicode.com/todos')
   //return fetch('https://gorest.co.in/public-api/users')
   return fetch('http://dummy.restapiexample.com/api/v1/employees')
    .then((response) => response.json())
    .then( (responseJson) => {
        this.setState({
          isLoading: false,
          dataSource: responseJson.data,
        })
    } )

    .catch((error)=> {
        console.log(error)
    });
  }


  onPressButton = (id) => {
    Alert.alert('Confirm Delete')
    var url = 'http://dummy.restapiexample.com/api/v1/employees'+id;
    axios.delete(url)
    .then(res => console.log(res.data));
    
 }

  render(){
    if(this.state.isLoading){
      return(
        <View style={styles.container}>
          <ActivityIndicator/>
          </View>
      )
    }
else{
  let data = this.state.dataSource.map((val,key) => {
    return <View key = {key} style={styles.item}>
      <Text>{"EMPLOYEE ID:  "+val.id}</Text>
      <Text>{"EMPLOYEE NAME:  "+val.employee_name}</Text>
      <Text>{"EMPLOYEE SALARY:  "+val.employee_salary}</Text>
      <Text>{"EMPLOYEE AGE:  "+val.employee_age}</Text>
      <Button onPress={() => { this.onPressButton(val.id) }} title="Delete" color="#ff0000" />
      
      </View>
  });
  return (
    <View>
     <Text style={backgroundColor='#d3d3d3'}>EMPLOYEES</Text>
      <ScrollView>
      {data}
    </ScrollView>
    </View>
    
   
  );
  }
}
}
const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
  item: {
    flex: 1,
    margin: 10,
    alignItems: 'center',
    justifyContent: 'center',
    borderBottomWidth: 1,
    borderBottomColor: '#eee'

  }
});
