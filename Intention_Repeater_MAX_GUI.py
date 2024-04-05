import tkinter as tk
from tkinter import ttk
import subprocess

def start_repeater():
    intention = intention_entry.get("1.0", tk.END).strip()
    duration = duration_entry.get()
    ram_usage = ram_slider.get()
    frequency = frequency_entry.get()
    suffix = suffix_var.get()
    use_hololink = hololink_var.get()
    boost_level = boost_level_entry.get()
    use_compression = compression_var.get()
    use_hashing = hashing_var.get()
    filename = filename_entry.get()

    command = ["intention_repeater_max.exe"]
    if filename:
        command.extend(["--file", "\"" + filename + "\""])
    elif intention:
        command.extend(["--intent", "\"" + intention + "\""])
    if duration:
        command.extend(["--dur", "\"" + duration + "\""])
    if ram_usage:
        command.extend(["--imem", str(ram_usage)])
    if frequency:
        command.extend(["--freq", "\"" + frequency + "\""])
    if suffix:
        command.extend(["--suffix", "\"" + suffix + "\""])
    if use_hololink:
        command.append("--usehololink")
    if boost_level:
        command.extend(["--boostlevel", boost_level])
    if use_compression:
        command.extend(["--compress", "y"])
    else:
        command.extend(["--compress", "n"])
    if use_hashing:
        command.extend(["--hashing", "y"])
    else:
        command.extend(["--hashing", "n"])

    # Print the command 
    print("Command:", " ".join(command)) 

    # Launch the process
    global process
    process = subprocess.Popen(command)

def stop_repeater():
    if process:
        process.terminate()

def update_ram_label(val):
    global ram_value_label
    if 'ram_value_label' not in globals():
        ram_value_label = ttk.Label(parameter_frame, text="")
        ram_value_label.grid(row=1, column=2, padx=5, pady=5)
    ram_value_label.config(text=f"{ram_slider.get():.1f} GB")

# Main Window
root = tk.Tk()
root.title("Intention Repeater MAX GUI")

# Intention Input Frame
intention_frame = ttk.Frame(root)
intention_frame.pack(padx=10, pady=10)

intention_label = ttk.Label(intention_frame, text="Intention:")
intention_label.pack(side=tk.LEFT)

intention_entry = tk.Text(intention_frame, height=3)
intention_entry.pack(side=tk.LEFT)

# Filename Input
filename_label = ttk.Label(intention_frame, text="Filename (optional):")
filename_label.pack(side=tk.LEFT, padx=5)

filename_entry = ttk.Entry(intention_frame)
filename_entry.pack(side=tk.LEFT)

# Parameter Frame
parameter_frame = ttk.Frame(root)
parameter_frame.pack(padx=10, pady=10)

# Duration
duration_label = ttk.Label(parameter_frame, text="Duration (HH:MM:SS) [Optional]:")
duration_label.grid(row=0, column=0, padx=5, pady=5)
duration_entry = ttk.Entry(parameter_frame)
duration_entry.grid(row=0, column=1, padx=5, pady=5)

# RAM Usage
ram_label = ttk.Label(parameter_frame, text="RAM Usage (GB):")
ram_label.grid(row=1, column=0, padx=5, pady=5)

ram_slider = ttk.Scale(parameter_frame, from_=1.0, to=8.0, orient=tk.HORIZONTAL, command=update_ram_label)
ram_slider.set(1.0)
ram_slider.grid(row=1, column=1, padx=5, pady=5)

ram_value_label = ttk.Label(parameter_frame, text="1.0 GB")
ram_value_label.grid(row=1, column=2, padx=5, pady=5)

# Frequency
frequency_label = ttk.Label(parameter_frame, text="Frequency (Hz) [Optional]:")
frequency_label.grid(row=2, column=0, padx=5, pady=5)
frequency_entry = ttk.Entry(parameter_frame)
frequency_entry.grid(row=2, column=1, padx=5, pady=5)

# Suffix
suffix_label = ttk.Label(parameter_frame, text="Suffix:")
suffix_label.grid(row=3, column=0, padx=5, pady=5)
suffix_var = tk.StringVar(value="HZ")
suffix_option = ttk.OptionMenu(parameter_frame, suffix_var, "HZ", "HZ", "EXP")
suffix_option.grid(row=3, column=1, padx=5, pady=5)

# Holo-Link
hololink_var = tk.BooleanVar()
hololink_check = ttk.Checkbutton(parameter_frame, text="Use Holo-Link", variable=hololink_var)
hololink_check.grid(row=4, column=0, columnspan=2, padx=5, pady=5)

# Boosting
boost_label = ttk.Label(parameter_frame, text="Boost Level [Optional]:")
boost_label.grid(row=5, column=0, padx=5, pady=5)
boost_level_entry = ttk.Entry(parameter_frame)
boost_level_entry.grid(row=5, column=1, padx=5, pady=5)

# Compression
compression_var = tk.BooleanVar()
compression_check = ttk.Checkbutton(parameter_frame, text="Use Compression", variable=compression_var)
compression_check.grid(row=6, column=0, columnspan=2, padx=5, pady=5)

# Hashing 
hashing_var = tk.BooleanVar()
hashing_check = ttk.Checkbutton(parameter_frame, text="Use Hashing", variable=hashing_var)
hashing_check.grid(row=7, column=0, columnspan=2, padx=5, pady=5) 
 
# Control Buttons Frame 
button_frame = ttk.Frame(root)
button_frame.pack(pady=10)

start_button = ttk.Button(button_frame, text="Start", command=start_repeater) 
start_button.pack(side=tk.LEFT, padx=5)

stop_button = ttk.Button(button_frame, text="Stop", command=stop_repeater)
stop_button.pack(side=tk.LEFT)

process = None 

root.mainloop()