from flask import Flask, request, jsonify

app = Flask(_name_)

dummy_data = [
    {
        "temperature": 25,
        "humidity": 80,
        "timestamp": "2022-11-25 22:10:00"
    },
    {
        "temperature": 26,
        "humidity": 85,
        "timestamp": "2022-11-25 22:10:05"
    }]

@app.route('/hilman/led_sensor/data', methods=['POST', 'GET'])
def sensor_data():
    if request.method == 'POST':
        data = request.get_json()
        print(data)
        return jsonify({'message': 'Data received!'})
    else:
        return jsonify(dummy_data)
    

if _name_ == '_main_':
    app.run(host='0.0.0.0', port=5000, debug=True)