import psutil
import time
import serial
import serial.tools.list_ports
import subprocess
import os

def find_cyd_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        desc = port.description.lower()
        if any(chip in desc for chip in ['ch340', 'cp210', 'usb serial', 'usb-serial', 'uart', 'esp32']):
            print(f"Found CYD on {port.device}")
            return port.device

com = None
while com is None:
    com = find_cyd_port()
    if com is None:
        print("CYD not found. Waiting for USB connection...")
        time.sleep(2)

ser = serial.Serial(com, 115200)

disk_path = 'C:/' if os.name == 'nt' else '/'

psutil.cpu_percent(interval=None)

net_io = psutil.net_io_counters()
last_recv = net_io.bytes_recv
last_sent = net_io.bytes_sent
last_time = time.time()

time.sleep(2)

def get_gpu_load():
    if os.name != 'nt':
        time.sleep(1)
        return 0.0
    cmd = r'typeperf "\GPU Engine(*engtype_3D)\Utilization Percentage" -sc 1'
    try:
        out = subprocess.check_output(cmd, shell=True).decode('cp866', errors='ignore')

        lines = out.splitlines()
        parts = lines[2].split(',')

        max_gpu = 0.0

        for p in parts[1:]:
            val = float(p.strip('"'))
            if val > max_gpu:
                max_gpu = val

        return round(min(max_gpu, 100.0), 1)
    except:
        return 0.0

while True:
    gpu = get_gpu_load()
    cpu = psutil.cpu_percent(interval=None)
    ram = psutil.virtual_memory().percent
    disk = psutil.disk_usage(disk_path).percent
    net_io = psutil.net_io_counters()
    now_time = time.time()
    dt = now_time - last_time

    net_down = round((net_io.bytes_recv - last_recv) / dt / (1024 * 1024), 1)
    net_up = round((net_io.bytes_sent - last_sent) / dt / (1024 * 1024), 1)

    last_recv = net_io.bytes_recv
    last_sent = net_io.bytes_sent
    last_time = now_time
    
    data = f"{cpu};{ram};{disk};{gpu};{net_down};{net_up}\n"
    ser.write(data.encode())
