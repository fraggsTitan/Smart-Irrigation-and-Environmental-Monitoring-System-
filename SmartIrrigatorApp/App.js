import React, { useState, useEffect } from "react";
import { View, Text, TextInput, Button, StyleSheet, ScrollView, Alert } from "react-native";
import AsyncStorage from "@react-native-async-storage/async-storage";

export default function App() {
  const [ip, setIp] = useState("");
  const [esp32Ip, setEsp32Ip] = useState("");
  const [sensorData, setSensorData] = useState("Loading...");
  const [soil, setSoil] = useState("");
  const [temp, setTemp] = useState("");
  const [hum, setHum] = useState("");
  const [pump, setPump] = useState("");

  useEffect(() => {
    (async () => {
      const savedIp = await AsyncStorage.getItem("esp32_ip");
      if (savedIp) {
        setEsp32Ip(savedIp);
        fetchData(savedIp);
      }
    })();
  }, []);

  useEffect(() => {
    if (esp32Ip) {
      const interval = setInterval(() => fetchData(esp32Ip), 5000);
      return () => clearInterval(interval);
    }
  }, [esp32Ip]);

  async function fetchData(currentIp) {
    try {
      const res = await fetch(`${currentIp}/setThresholds`);
      const json = await res.json();
      setSensorData(JSON.stringify(json, null, 2));
    } catch (err) {
      setSensorData("Error fetching data");
    }
  }

  async function saveIp() {
    const formatted = `http://${ip.trim()}`;
    await AsyncStorage.setItem("esp32_ip", formatted);
    setEsp32Ip(formatted);
    fetchData(formatted);
    Alert.alert("IP Saved", `ESP32 IP set to: ${formatted}`);
  }

  async function updateThresholds() {
    const body = {};
    if (soil) body.soil = parseInt(soil);
    if (temp) body.temp = parseInt(temp);
    if (hum) body.hum = parseInt(hum);
    if (pump) body.pump = pump === "true";

    try {
      const res = await fetch(`${esp32Ip}/updateThresholds`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });
      const text = await res.text();
      Alert.alert("Server Response", text);
      fetchData(esp32Ip);
    } catch (err) {
      Alert.alert("Error", "Failed to update thresholds");
    }
  }

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Text style={styles.title}>🌱 Smart Irrigator Dashboard</Text>

      <View style={styles.card}>
        <Text style={styles.subtitle}>Set ESP32 IP</Text>
        <TextInput
          style={styles.input}
          placeholder="192.168.1.50"
          value={ip}
          onChangeText={setIp}
        />
        <Button title="Save IP" onPress={saveIp} />
      </View>

      <View style={styles.card}>
        <Text style={styles.subtitle}>Live Sensor Data</Text>
        <Text style={styles.dataBox}>{sensorData}</Text>
      </View>

      <View style={styles.card}>
        <Text style={styles.subtitle}>Update Thresholds</Text>
        <TextInput
          style={styles.input}
          placeholder="Soil Threshold"
          keyboardType="numeric"
          value={soil}
          onChangeText={setSoil}
        />
        <TextInput
          style={styles.input}
          placeholder="Temp Threshold"
          keyboardType="numeric"
          value={temp}
          onChangeText={setTemp}
        />
        <TextInput
          style={styles.input}
          placeholder="Humidity Threshold"
          keyboardType="numeric"
          value={hum}
          onChangeText={setHum}
        />
        <TextInput
          style={styles.input}
          placeholder="Pump (true/false)"
          value={pump}
          onChangeText={setPump}
        />
        <Button title="Update" onPress={updateThresholds} />
      </View>
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    padding: 20,
    backgroundColor: "#f4f4f4",
  },
  title: {
    fontSize: 24,
    fontWeight: "bold",
    color: "#2c3e50",
    marginBottom: 20,
    textAlign: "center",
  },
  card: {
    backgroundColor: "#fff",
    padding: 15,
    borderRadius: 10,
    marginBottom: 20,
    shadowColor: "#000",
    shadowOpacity: 0.1,
    shadowRadius: 6,
    elevation: 3,
  },
  subtitle: {
    fontSize: 18,
    fontWeight: "600",
    marginBottom: 10,
    color: "#34495e",
  },
  input: {
    borderWidth: 1,
    borderColor: "#ccc",
    borderRadius: 6,
    padding: 10,
    marginBottom: 10,
    backgroundColor: "#fafafa",
  },
  dataBox: {
    fontFamily: "monospace",
    backgroundColor: "#eef",
    padding: 10,
    borderRadius: 6,
    color: "#2c3e50",
  },
});