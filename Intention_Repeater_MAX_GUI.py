import os
import subprocess
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import psutil

class IntentionRepeaterGUI:
    def __init__(self, master):
        self.master = master
        master.title("Intention Repeater MAX GUI")
        self.process = None

        # Create input fields and labels
        self.intention_label = ttk.Label(master, text="Intention:")
        self.intention_label.grid(row=0, column=0, padx=5, pady=5, sticky="e")
        self.intention_entry = ttk.Entry(master, width=50)
        self.intention_entry.grid(row=0, column=1, padx=5, pady=5)

        self.duration_label = ttk.Label(master, text="Duration (HH:MM:SS):")
        self.duration_label.grid(row=1, column=0, padx=5, pady=5, sticky="e")
        self.duration_entry = ttk.Entry(master, width=20)
        self.duration_entry.grid(row=1, column=1, padx=5, pady=5, sticky="w")

        self.ram_label = ttk.Label(master, text="RAM Usage (GB):")
        self.ram_label.grid(row=2, column=0, padx=5, pady=5, sticky="e")
        self.ram_entry = ttk.Entry(master, width=10)
        self.ram_entry.insert(0, "1.0")
        self.ram_entry.grid(row=2, column=1, padx=5, pady=5, sticky="w")

        self.freq_label = ttk.Label(master, text="Frequency (Hz):")
        self.freq_label.grid(row=3, column=0, padx=5, pady=5, sticky="e")
        self.freq_entry = ttk.Entry(master, width=10)
        self.freq_entry.insert(0, "0")
        self.freq_entry.grid(row=3, column=1, padx=5, pady=5, sticky="w")

        self.boost_label = ttk.Label(master, text="Boost Level (1-100):")
        self.boost_label.grid(row=4, column=0, padx=5, pady=5, sticky="e")
        self.boost_entry = ttk.Entry(master, width=10)
        self.boost_entry.insert(0, "0")
        self.boost_entry.grid(row=4, column=1, padx=5, pady=5, sticky="w")

        self.amplify_label = ttk.Label(master, text="Amplification:")
        self.amplify_label.grid(row=5, column=0, padx=5, pady=5, sticky="e")
        self.amplify_entry = ttk.Entry(master, width=20)
        self.amplify_entry.insert(0, "1000000000")
        self.amplify_entry.grid(row=5, column=1, padx=5, pady=5, sticky="w")

        self.rest_every_label = ttk.Label(master, text="Rest Every (seconds):")
        self.rest_every_label.grid(row=6, column=0, padx=5, pady=5, sticky="e")
        self.rest_every_entry = ttk.Entry(master, width=10)
        self.rest_every_entry.insert(0, "0")
        self.rest_every_entry.grid(row=6, column=1, padx=5, pady=5, sticky="w")

        self.rest_for_label = ttk.Label(master, text="Rest For (seconds):")
        self.rest_for_label.grid(row=7, column=0, padx=5, pady=5, sticky="e")
        self.rest_for_entry = ttk.Entry(master, width=10)
        self.rest_for_entry.insert(0, "0")
        self.rest_for_entry.grid(row=7, column=1, padx=5, pady=5, sticky="w")

        self.file_label = ttk.Label(master, text="Intention File:")
        self.file_label.grid(row=8, column=0, padx=5, pady=5, sticky="e")
        self.file_entry = ttk.Entry(master, width=50)
        self.file_entry.grid(row=8, column=1, padx=5, pady=5)
        self.file_button = ttk.Button(master, text="Browse", command=self.browse_file)
        self.file_button.grid(row=8, column=2, padx=5, pady=5)

         # Create checkboxes
        self.holo_var = tk.BooleanVar()
        self.holo_checkbox = ttk.Checkbutton(master, text="Use Holo-Link", variable=self.holo_var)
        self.holo_checkbox.grid(row=9, column=0, padx=5, pady=5, sticky="w")

        self.hashing_var = tk.BooleanVar()
        self.hashing_checkbox = ttk.Checkbutton(master, text="Use Hashing", variable=self.hashing_var)
        self.hashing_checkbox.grid(row=9, column=1, padx=5, pady=5, sticky="w")

        self.compress_var = tk.BooleanVar()
        self.compress_checkbox = ttk.Checkbutton(master, text="Use Compression", variable=self.compress_var)
        self.compress_checkbox.grid(row=9, column=2, padx=5, pady=5, sticky="w")

        # Create color dropdown
        self.color_label = ttk.Label(master, text="Color:")
        self.color_label.grid(row=10, column=0, padx=5, pady=5, sticky="e")
        self.color_var = tk.StringVar()
        self.color_dropdown = ttk.Combobox(master, textvariable=self.color_var, values=[
            "WHITE",
            "BLACK",
            "BLUE",
            "CYAN",
            "DARKGRAY",
            "GREEN",
            "LIGHTBLUE",
            "LIGHTCYAN",
            "LIGHTGRAY",
            "LIGHTGREEN",
            "LIGHTMAGENTA",
            "LIGHTRED",
            "LIGHTYELLOW",
            "MAGENTA",
            "RED",
            "YELLOW"
        ])
        self.color_dropdown.current(0)
        self.color_dropdown.grid(row=10, column=1, padx=5, pady=5, sticky="w")

        # Create Holo-Link and Nesting Files buttons
        self.holo_link_button = ttk.Button(master, text="Create Holo-Link Files", command=self.create_holo_link_files)
        self.holo_link_button.grid(row=11, column=0, padx=5, pady=5)

        self.nesting_files_button = ttk.Button(master, text="Create Nesting Files", command=self.create_nesting_files)
        self.nesting_files_button.grid(row=11, column=1, padx=5, pady=5)

        # Create suffix dropdown
        self.suffix_label = ttk.Label(master, text="Suffix:")
        self.suffix_label.grid(row=12, column=0, padx=5, pady=5, sticky="e")
        self.suffix_var = tk.StringVar()
        self.suffix_dropdown = ttk.Combobox(master, textvariable=self.suffix_var, values=["HZ", "EXP"])
        self.suffix_dropdown.current(0)
        self.suffix_dropdown.grid(row=12, column=1, padx=5, pady=5, sticky="w")

        # Create timer dropdown
        self.timer_label = ttk.Label(master, text="Timer:")
        self.timer_label.grid(row=13, column=0, padx=5, pady=5, sticky="e")
        self.timer_var = tk.StringVar()
        self.timer_dropdown = ttk.Combobox(master, textvariable=self.timer_var, values=["EXACT", "INEXACT"])
        self.timer_dropdown.current(0)
        self.timer_dropdown.grid(row=13, column=1, padx=5, pady=5, sticky="w")

        # Create buttons
        self.run_button = ttk.Button(master, text="Run", command=self.run_intention_repeater)
        self.run_button.grid(row=14, column=0, padx=5, pady=10)

        self.quit_button = ttk.Button(master, text="Quit", command=self.quit_program)
        self.quit_button.grid(row=14, column=1, padx=5, pady=10)

    def browse_file(self):
        file_path = filedialog.askopenfilename()
        self.file_entry.delete(0, tk.END)
        self.file_entry.insert(0, file_path)

    def run_intention_repeater(self):
        intention = self.intention_entry.get()
        duration = self.duration_entry.get()
        ram = self.ram_entry.get()
        freq = self.freq_entry.get()
        boost = self.boost_entry.get()
        amplify = self.amplify_entry.get()
        rest_every = self.rest_every_entry.get()
        rest_for = self.rest_for_entry.get()
        file_path = self.file_entry.get()
        use_holo = self.holo_var.get()
        use_hashing = self.hashing_var.get()
        use_compress = self.compress_var.get()
        color = self.color_var.get()
        suffix = self.suffix_var.get()
        timer = self.timer_var.get()

        command = ["intention_repeater_max.exe"]

        if intention:
            command.extend(["--intent", "\"" + intention + "\""])
        if duration:
            command.extend(["--dur", "\"" + duration + "\""])
        if ram:
            command.extend(["--imem", ram])
        if freq:
            command.extend(["--freq", freq])
        if boost:
            command.extend(["--boostlevel", boost])
        if amplify:
            command.extend(["--amplify", amplify])
        if rest_every:
            command.extend(["--restevery", rest_every])
        if rest_for:
            command.extend(["--restfor", rest_for])
        if file_path:
            command.extend(["--file", "\"" + file_path + "\""])
        if use_holo:
            command.append("--usehololink")
        if use_hashing:
            command.extend(["--hashing", "y"])
        else:
            command.extend(["--hashing", "n"])
        if use_compress:
            command.extend(["--compress", "y"])
        else:
            command.extend(["--compress", "n"])
        if color:
            command.extend(["--color", color])
        if suffix:
            command.extend(["--suffix", suffix])
        if timer:
            command.extend(["--timer", timer])

        command_str = " ".join(command)

        try:
            self.process = subprocess.Popen(command_str)
            self.run_button.config(state=tk.DISABLED)
        except FileNotFoundError:
            messagebox.showerror("Error", "intention_repeater_max.exe not found.")
    
    def quit_program(self):
        if self.process:
            try:
                process = psutil.Process(self.process.pid)
                for proc in process.children(recursive=True):
                    proc.kill()
                process.kill()
                print("\n")  # Print a newline in the terminal
            except psutil.NoSuchProcess:
                pass
        self.master.quit()

    def create_holo_link_files(self):
        try:
            subprocess.run(["intention_repeater_max.exe", "--createhololinkfiles"])
        except FileNotFoundError:
            messagebox.showerror("Error", "intention_repeater_max.exe not found.")

    def create_nesting_files(self):
        try:
            subprocess.run(["intention_repeater_max.exe", "--createnestingfiles"])
        except FileNotFoundError:
            messagebox.showerror("Error", "intention_repeater_max.exe not found.")

root = tk.Tk()
gui = IntentionRepeaterGUI(root)
root.mainloop()