import os
import subprocess
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
import psutil
import ttkbootstrap as ttk
from ttkbootstrap.constants import *
from ttkbootstrap.tooltip import ToolTip
import threading
import sys

class IntentionRepeaterGUI:
    def disable_widgets(self):
        # Assuming widget_variable_names is an array of widget names as strings
        widget_variable_names = [
            'suffix_var', 'rest_every_entry', 'nesting_files_button', 'duration_label',
            'color_label', 'file_button', 'compress_checkbox', 'boost_label', 'file_label',
            'rest_for_label', 'compress_var', 'holo_var', 'freq_label', 'color_dropdown',
            'timer_var', 'rest_for_entry', 'run_button', 'amplify_label', 'holo_link_button',
            'hashing_checkbox', 'boost_entry', 'intention_entry', 'ram_entry', 'ram_label',
            'rest_every_label', 'timer_label', 'hashing_var', 'suffix_label', 'intention_label',
            'file_entry', 'holo_checkbox', 'quit_button', 'amplify_entry', 'suffix_dropdown',
            'duration_entry', 'timer_dropdown', 'freq_entry', 'color_var'
        ]

        for widget_name in widget_variable_names:
            if widget_name != 'quit_button':  # Assuming 'quit_button' is the name of your Quit button
                widget = getattr(self, widget_name)  # Get the widget object by name
                if hasattr(widget, 'config'):
                    widget.config(state="disabled")


    def create_context_menu(self, entry_widget):
        context_menu = tk.Menu(self.master, tearoff=0)
        context_menu.add_command(label="Copy", command=lambda: self.master.clipboard_clear() or self.master.clipboard_append(entry_widget.get()))
        context_menu.add_command(label="Paste", command=lambda: entry_widget.insert(tk.INSERT, self.master.clipboard_get()))
        entry_widget.bind("<Button-3>", lambda event: context_menu.post(event.x_root, event.y_root))


    def __init__(self, master):
        self.master = master
        master.title("Intention Repeater MAX GUI")
        master.resizable(False, False)  # Make the window not resizable
        self.process = None

        # Bind the close event to the on_close function
        master.protocol("WM_DELETE_WINDOW", self.on_close)

        # Create input fields and labels
        self.intention_label = ttk.Label(master, text="Intention:")
        self.intention_label.grid(row=0, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.intention_label, "The intention to Repeat.")

        self.intention_entry = tk.Text(master, width=75, height=5)
        self.intention_entry.grid(row=0, column=1, columnspan=3, padx=5, pady=5, sticky="w")

        self.file_label = ttk.Label(master, text="Intention File:")
        self.file_label.grid(row=1, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.file_label, text = "The File to Repeat. Will replace the intention with the contents of the file.")
        self.file_entry = ttk.Entry(master, width=55)
        self.file_entry.grid(row=1, column=1, columnspan=2, padx=5, pady=5, sticky="w")
        self.file_button = ttk.Button(master, text="Browse", command=self.browse_file)
        self.file_button.grid(row=1, column=3, padx=5, pady=5, sticky="e")

        self.duration_label = ttk.Label(master, text="Duration (HH:MM:SS):")
        self.duration_label.grid(row=2, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.duration_label, text = "The duration to repeat the intention for. Optional.")
        self.duration_entry = ttk.Entry(master, width=11)
        self.duration_entry.grid(row=2, column=1, padx=5, pady=5, sticky="w")

        self.ram_label = ttk.Label(master, text="RAM Usage (GB):")
        self.ram_label.grid(row=3, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.ram_label, text = "The amount of RAM to use while repeating the intention.")
        self.ram_entry = ttk.Entry(master, width=11)
        self.ram_entry.insert(0, "1.0")
        self.ram_entry.grid(row=3, column=1, padx=5, pady=5, sticky="w")

        self.freq_label = ttk.Label(master, text="Frequency (Hz):")
        self.freq_label.grid(row=4, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.freq_label, text = "The frequency to repeat the intention at. Optional.")
        self.freq_entry = ttk.Entry(master, width=11)
        self.freq_entry.insert(0, "0")
        self.freq_entry.grid(row=4, column=1, padx=5, pady=5, sticky="w")

        self.boost_label = ttk.Label(master, text="Boost Level (1-100):")
        self.boost_label.grid(row=5, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.boost_label, text = "The Nesting Level to use while repeating the intention. Uses INTENTIONS.TXT and NEST-Files. Optional.")
        self.boost_entry = ttk.Entry(master, width=11)
        self.boost_entry.insert(0, "0")
        self.boost_entry.grid(row=5, column=1, padx=5, pady=5, sticky="w")

        self.nesting_files_button = ttk.Button(master, text="Create Nesting Files", command=self.create_nesting_files)
        self.nesting_files_button.grid(row=5, column=2, columnspan=2, padx=5, pady=5, sticky="w")

        self.amplify_label = ttk.Label(master, text="Amplification:")
        self.amplify_label.grid(row=6, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.amplify_label, text = "The amplification to use while repeating the intention. Works with INEXACT Timer. Optional.")
        self.amplify_entry = ttk.Entry(master, width=11)
        self.amplify_entry.insert(0, "1000000000")
        self.amplify_entry.grid(row=6, column=1, padx=5, pady=5, sticky="w")

        self.rest_every_label = ttk.Label(master, text="Rest Every (seconds):")
        self.rest_every_label.grid(row=7, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.rest_every_label, text = "Rest every mentioned number of seconds. Optional.")
        self.rest_every_entry = ttk.Entry(master, width=11)
        self.rest_every_entry.insert(0, "0")
        self.rest_every_entry.grid(row=7, column=1, padx=5, pady=5, sticky="w")

        self.rest_for_label = ttk.Label(master, text="Rest For (seconds):")
        self.rest_for_label.grid(row=8, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.rest_for_label, text = "The amount of time to rest after the last repetition. Optional.")
        self.rest_for_entry = ttk.Entry(master, width=11)
        self.rest_for_entry.insert(0, "0")
        self.rest_for_entry.grid(row=8, column=1, padx=5, pady=5, sticky="w")

        # Create checkboxes
        self.holo_var = tk.BooleanVar()
        self.holo_checkbox = ttk.Checkbutton(master, text="Use Holo-Link", variable=self.holo_var)
        self.holo_checkbox.grid(row=9, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.holo_checkbox, text = "Use Holo-Link. References HSUPLINK.TXT and INTENTIONS.TXT.")

        self.holo_link_button = ttk.Button(master, text="Create Holo-Link Files", command=self.create_holo_link_files)
        self.holo_link_button.grid(row=9, column=1, padx=5, pady=5, sticky="w")

        self.hashing_var = tk.BooleanVar()
        self.hashing_checkbox = ttk.Checkbutton(master, text="Use Hashing", variable=self.hashing_var)
        self.hashing_checkbox.grid(row=9, column=2, padx=5, pady=5, sticky="w")
        ToolTip(self.hashing_checkbox, text = "Use Hashing to multiply the repetition rate.")

        self.compress_var = tk.BooleanVar()
        self.compress_checkbox = ttk.Checkbutton(master, text="Use Compression", variable=self.compress_var)
        self.compress_checkbox.grid(row=9, column=3, padx=5, pady=5, sticky="w")
        ToolTip(self.compress_checkbox, text = "Use Compression to increase the repetition rate.")

        # Create color dropdown
        self.color_label = ttk.Label(master, text="Color:")
        self.color_label.grid(row=10, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.color_label, text = "The text color to use.")
        self.color_var = tk.StringVar()
        self.color_dropdown = ttk.Combobox(master, textvariable=self.color_var, state="readonly", width=15, values=[
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

        # Create suffix dropdown
        self.suffix_label = ttk.Label(master, text="Suffix:")
        self.suffix_label.grid(row=11, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.suffix_label, text = "The suffix to use for displaying repetition rate.")
        self.suffix_var = tk.StringVar()
        self.suffix_dropdown = ttk.Combobox(master, textvariable=self.suffix_var, state="readonly", width=15, values=["HZ", "EXP"])
        self.suffix_dropdown.current(0)
        self.suffix_dropdown.grid(row=11, column=1, padx=5, pady=5, sticky="w")

        # Create timer dropdown
        self.timer_label = ttk.Label(master, text="Timer:")
        self.timer_label.grid(row=12, column=0, padx=5, pady=5, sticky="e")
        ToolTip(self.timer_label, text = "EXACT is more accurate clock. INEXACT is faster.")
        self.timer_var = tk.StringVar()
        self.timer_dropdown = ttk.Combobox(master, textvariable=self.timer_var, state="readonly", width=15, values=["EXACT", "INEXACT"])
        self.timer_dropdown.current(0)
        self.timer_dropdown.grid(row=12, column=1, padx=5, pady=5, sticky="w")

        # Create buttons
        self.run_button = ttk.Button(master, text="Run", command=self.run_intention_repeater)
        self.run_button.grid(row=12, column=2, padx=5, pady=10, sticky="e")

        self.quit_button = ttk.Button(master, text="Quit", command=self.quit_program)
        self.quit_button.grid(row=12, column=3, padx=5, pady=10, sticky="w")

    def on_run_clicked(self):
        self.disable_widgets()  # Disable widgets
        # Your existing code to launch Intention_Repeater_MAX goes here

    def browse_file(self):
        file_path = filedialog.askopenfilename()
        self.file_entry.delete(0, tk.END)
        self.file_entry.insert(0, file_path)

    def run_intention_repeater(self):
        intention = self.intention_entry.get("1.0", tk.END).strip()
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

        if not intention and boost == "0" and not use_holo and not file_path:
            messagebox.showinfo("Missing Fields", "Intention or File must be filled out. Or Holo-Link or Boosting must be selected.")
            return

        if boost != "0":
            nest_files = [f for f in os.listdir(".") if f.startswith("NEST")]
            if not nest_files:
                messagebox.showinfo("Missing Files", "Missing Boosting Files. Click Create Nesting Files to generate.")
                return

        if use_holo:
            if not os.path.isfile("HSUPLINK.TXT"):
                messagebox.showinfo("Missing Files", "Missing Holo-Link files. Click Create Holo-Link Files to generate.")
                return

        if boost != "0" or use_holo:
            if not os.path.isfile("INTENTIONS.TXT"):
                messagebox.showinfo("Missing Files", "Missing INTENTIONS.TXT. Click OK to create and edit.")
                open("INTENTIONS.TXT", "w").close()

                if sys.platform == "win32":
                    # Windows
                    subprocess.run(["notepad", "INTENTIONS.TXT"])
                elif sys.platform == "darwin":
                    # macOS
                    subprocess.run(["open", "-e", "INTENTIONS.TXT"])
                else:
                    # Linux
                    editors = ["gedit", "kate", "mousepad", "leafpad", "pluma", "geany", "kwrite", "nano", "vim"]
                    selected_editor = None
                    for editor in editors:
                        if os.system(f"which {editor} >/dev/null 2>&1") == 0:
                            selected_editor = editor
                            break

                    if selected_editor:
                        subprocess.run([selected_editor, "INTENTIONS.TXT"])
                    else:
                        messagebox.showinfo("Editor Not Found", "No suitable text editor found. Please manually edit INTENTIONS.TXT.")
                        os.system("xdg-open INTENTIONS.TXT")
            else:
                response = messagebox.askquestion("Edit INTENTIONS.TXT", "INTENTIONS.TXT already exists. Do you want to edit it?")
                if response == "yes":
                    if sys.platform == "win32":
                        # Windows
                        subprocess.run(["notepad", "INTENTIONS.TXT"])
                    elif sys.platform == "darwin":
                        # macOS
                        subprocess.run(["open", "-e", "INTENTIONS.TXT"])
                    else:
                        # Linux
                        editors = ["gedit", "kate", "mousepad", "leafpad", "pluma", "geany", "kwrite", "nano", "vim"]
                        selected_editor = None
                        for editor in editors:
                            if os.system(f"which {editor} >/dev/null 2>&1") == 0:
                                selected_editor = editor
                                break

                        if selected_editor:
                            subprocess.run([selected_editor, "INTENTIONS.TXT"])
                        else:
                            messagebox.showinfo("Editor Not Found", "No suitable text editor found. Please manually edit INTENTIONS.TXT.")
                            os.system("xdg-open INTENTIONS.TXT")
                    
        self.on_run_clicked()
        
        executable_name = "Intention_Repeater_MAX"

        cwdstr = os.getcwd() + "/" + executable_name
        
        if not os.path.isfile(cwdstr):
            messagebox.showerror("Error", f"{cwdstr} not found when attempting to run.")
            return

        command = [cwdstr]

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
        cwdstr = command_str
        print(cwdstr)

        try:
            if sys.platform == "win32":
                # For Windows
                command = f"start cmd /c {cwdstr}"
                subprocess.Popen(command, shell=True)
            elif sys.platform == "darwin":
                # For macOS
                command = f'osascript -e \'tell application "Terminal" to do script "{cwdstr}"\''
                subprocess.Popen(command, shell=True)
            else:
                # For Linux
                terminal_commands = [
                    "gnome-terminal -- bash -c '{} ; exec bash'",  # GNOME Terminal
                    "xterm -e bash -c '{} ; exec bash'",  # Xterm
                    "konsole -e bash -c '{} ; exec bash'",  # Konsole
                    "xfce4-terminal -e bash -c '{} ; exec bash'",  # Xfce Terminal
                    "lxterminal -e bash -c '{} ; exec bash'",  # LXTerminal
                    "mate-terminal -e bash -c '{} ; exec bash'",  # MATE Terminal
                    "terminology -e bash -c '{} ; exec bash'",  # Terminology
                    "tilix -e bash -c '{} ; exec bash'",  # Tilix
                    "qterminal -e bash -c '{} ; exec bash'",  # QTerminal
                    "urxvt -e bash -c '{} ; exec bash'",  # URxvt
                    "st -e bash -c '{} ; exec bash'",  # Simple Terminal
                    # Add more terminal commands as needed
                ]

                for cmd in terminal_commands:
                    try:
                        command = cmd.format(cwdstr)
                        break
                    except FileNotFoundError:
                        continue
                subprocess.Popen(command, shell=True)
                    
        except FileNotFoundError:
            messagebox.showerror("Error", cwdstr + " not found when trying to run with parameters.")
    
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
        executable_name = "Intention_Repeater_MAX"
        cwdstr = os.getcwd() + "/" + executable_name
        print(cwdstr)

        if not os.path.isfile(cwdstr):
            messagebox.showerror("Error", f"{cwdstr} not found creating Holo-Link files.")
            return

        try:
            if sys.platform == "win32":
                # For Windows
                command = f'start cmd /c "{cwdstr} --createhololinkfiles"'
                subprocess.Popen(command, shell=True)
            elif sys.platform == "darwin":
                # For macOS
                command = f'osascript -e \'tell application "Terminal" to do script "{cwdstr} --createhololinkfiles"\''
                subprocess.Popen(command, shell=True)
            else:
                # For Linux
                terminal_commands = [
                    "gnome-terminal -- bash -c '{} --createhololinkfiles; exec bash'",  # GNOME Terminal
                    "xterm -e bash -c '{} --createhololinkfiles; exec bash'",  # Xterm
                    "konsole -e bash -c '{} --createhololinkfiles; exec bash'",  # Konsole
                    "xfce4-terminal -e bash -c '{} --createhololinkfiles; exec bash'",  # Xfce Terminal
                    "lxterminal -e bash -c '{} --createhololinkfiles; exec bash'",  # LXTerminal
                    "mate-terminal -e bash -c '{} --createhololinkfiles; exec bash'",  # MATE Terminal
                    "terminology -e bash -c '{} --createhololinkfiles; exec bash'",  # Terminology
                    "tilix -e bash -c '{} --createhololinkfiles; exec bash'",  # Tilix
                    "qterminal -e bash -c '{} --createhololinkfiles; exec bash'",  # QTerminal
                    "urxvt -e bash -c '{} --createhololinkfiles; exec bash'",  # URxvt
                    "st -e bash -c '{} --createhololinkfiles; exec bash'",  # Simple Terminal
                    # Add more terminal commands as needed
                ]

                for cmd in terminal_commands:
                    try:
                        command = cmd.format(cwdstr)
                        subprocess.Popen(command, shell=True)
                        break
                    except FileNotFoundError:
                        continue

        except FileNotFoundError:
            messagebox.showerror("Error", f"{cwdstr} not found creating Holo-Link files.")

    def create_nesting_files(self):
        executable_name = "Intention_Repeater_MAX"
        cwdstr = os.getcwd() + "/" + executable_name
        print(cwdstr)

        if not os.path.isfile(cwdstr):
            messagebox.showerror("Error", f"{cwdstr} not found creating Nesting Files.")
            return

        try:
            if sys.platform == "win32":
                # For Windows
                command = f'start cmd /c "{cwdstr} --createnestingfiles"'
                subprocess.Popen(command, shell=True)
            elif sys.platform == "darwin":
                # For macOS
                command = f'osascript -e \'tell application "Terminal" to do script "{cwdstr} --createnestingfiles"\''
                subprocess.Popen(command, shell=True)
            else:
                # For Linux
                terminal_commands = [
                    "gnome-terminal -- bash -c '{} --createnestingfiles; exec bash'",  # GNOME Terminal
                    "xterm -e bash -c '{} --createnestingfiles; exec bash'",  # Xterm
                    "konsole -e bash -c '{} --createnestingfiles; exec bash'",  # Konsole
                    "xfce4-terminal -e bash -c '{} --createnestingfiles; exec bash'",  # Xfce Terminal
                    "lxterminal -e bash -c '{} --createnestingfiles; exec bash'",  # LXTerminal
                    "mate-terminal -e bash -c '{} --createnestingfiles; exec bash'",  # MATE Terminal
                    "terminology -e bash -c '{} --createnestingfiles; exec bash'",  # Terminology
                    "tilix -e bash -c '{} --createnestingfiles; exec bash'",  # Tilix
                    "qterminal -e bash -c '{} --createnestingfiles; exec bash'",  # QTerminal
                    "urxvt -e bash -c '{} --createnestingfiles; exec bash'",  # URxvt
                    "st -e bash -c '{} --createnestingfiles; exec bash'",  # Simple Terminal
                    # Add more terminal commands as needed
                ]

                for cmd in terminal_commands:
                    try:
                        command = cmd.format(cwdstr)
                        subprocess.Popen(command, shell=True)
                        break
                    except FileNotFoundError:
                        continue

        except FileNotFoundError:
            messagebox.showerror("Error", f"{cwdstr} not found.")

    def on_close(self):
        if self.process:
            process = psutil.Process(self.process.pid)
            for proc in process.children(recursive=True):
                proc.kill()
            process.kill()
        self.master.quit()

if __name__ == "__main__":
    root = tk.Tk()
    gui = IntentionRepeaterGUI(root)
    root.eval('tk::PlaceWindow . center')  # Open the GUI in the center of the screen
    root.mainloop()
