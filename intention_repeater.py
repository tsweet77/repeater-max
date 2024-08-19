import argparse
import time
import sys
import signal
from datetime import timedelta
import hashlib

def load_file_content(file_path):
    try:
        with open(file_path, 'rb') as file:
            return file.read().decode('utf-8', errors='ignore')
    except IOError:
        print(f"Error: Unable to read file {file_path}")
        sys.exit(1)

def format_number(num, is_frequency=False):
    if is_frequency:
        suffixes = ['Hz', 'kHz', 'MHz', 'GHz', 'THz', 'PHz', 'EHz', 'ZHz', 'YHz', 'RHz']
    else:
        suffixes = [' ', 'k', 'M', 'B', 'T', 'q', 'Q', 's', 'S', 'O', 'N', 'D']
    
    for suffix in suffixes:
        if num < 1000:
            return f"{num:.3f}{suffix}"
        num /= 1000
    return f"{num:.3f}{suffixes[-1]}"

def intention_repeater(intention, file1, file2, frequency, runtime, log_file, use_hashing):
    new_intention = intention
    if file1:
        new_intention += load_file_content(file1)
    if file2:
        new_intention += load_file_content(file2)

    new_intention2 = new_intention
    multiplier = 0
    hash_multiplier = 0

    print("Intention Repeater Python (c)2024 Anthro Teacher")
    print("www.intentionrepeater.com")
    print("Loading...", end='', flush=True)

    # First, multiply the intention
    while len(new_intention2) < 1024*1024*10:
        new_intention2 += new_intention
        multiplier += 1

    if use_hashing:
        # Then, hash the multiplied intention
        new_intention2 = hashlib.sha256(new_intention2.encode()).hexdigest()
        
        # Finally, multiply the hash
        original_hash = new_intention2
        while len(new_intention2) < 1024*1024*10:
            new_intention2 += original_hash
            hash_multiplier += 1

        new_intention2 = hashlib.sha256(new_intention2.encode()).hexdigest()
    else:
        # If not hashing, continue multiplying to reach 10MB
        while len(new_intention2) < 1024*1024*10:
            new_intention2 += new_intention
            multiplier += 1

    print("\n")

    iterations = 0
    start_time = time.time()
    last_output_time = start_time
    cycle_start_time = start_time
    cycle_iterations = 0

    print(f"Intention: {intention}")
    print(f"Frequency: {'MAX' if frequency == 0 else f'{frequency:.2f}Hz'} (x{multiplier} multiplier)")
    if use_hashing:
        print(f"Hashing: Enabled (x{hash_multiplier} hash multiplier)")
    print(f"Runtime: {'Infinite' if runtime == 0 else f'{runtime} seconds'}")
    print("\nPress Ctrl+C to stop the repeater.\n")

    try:
        while True:
            process_intention = new_intention2
            iterations += 1
            cycle_iterations += 1

            current_time = time.time()
            elapsed_time = current_time - start_time
            cycle_time = current_time - cycle_start_time

            if cycle_time >= 1:
                actual_frequency = cycle_iterations / cycle_time
                cycle_start_time = current_time
                cycle_iterations = 0

            if current_time - last_output_time >= 1:
                formatted_iterations = format_number(iterations * multiplier * (hash_multiplier if use_hashing else 1))
                formatted_frequency = format_number(actual_frequency, is_frequency=True)
                timer = str(timedelta(seconds=int(elapsed_time)))

                output = f"[{timer}] {formatted_iterations} iterations | {formatted_frequency}"
                if use_hashing:
                    output += f" (x{multiplier} Mult, x{hash_multiplier} Hash Mult)"
                else:
                    output += f" (x{multiplier} Mult)"
                print(f"\r{output}   ", end='', flush=True)
                
                if log_file:
                    with open(log_file, 'a') as f:
                        f.write(f"{output}\n")
                
                last_output_time = current_time

            if runtime > 0 and elapsed_time >= runtime:
                raise KeyboardInterrupt

            if frequency > 0:
                time.sleep(1 / frequency)

    except KeyboardInterrupt:
        print("\n\nIntention Repeater stopped.")
        print(f"Total runtime: {str(timedelta(seconds=int(time.time() - start_time)))}")
        print(f"Total iterations: {format_number(iterations * multiplier * (hash_multiplier if use_hashing else 1))}")

def get_user_input(args):
    while not args.intent:
        args.intent = input("Enter your intention (Required): ").strip()
        if not args.intent:
            print("Intention cannot be empty. Please try again.")
    
    if args.file is None:
        args.file = input("Enter the path to the first file (Optional, press Enter to skip): ").strip()
    elif args.file.lower() == 'none':
        args.file = None
    
    if args.file2 is None:
        args.file2 = input("Enter the path to the second file (Optional, press Enter to skip): ").strip()
    elif args.file2.lower() == 'none':
        args.file2 = None
    
    if args.freq is None:
        freq_input = input("Enter the frequency in Hz (0 = MAX, default is 3): ").strip()
        args.freq = 3 if not freq_input else float(''.join(c for c in freq_input if c.isdigit() or c == '.'))

    if args.runtime is None:
        runtime_input = input("Enter the total runtime in seconds (0 = Infinite, default is 0): ").strip()
        args.runtime = 0 if not runtime_input else int(runtime_input)

    if args.log is None:
        args.log = input("Enter the path for the log file (Optional, press Enter to skip): ").strip()
    elif args.log.lower() == 'none':
        args.log = None

    if args.hash is None:
        hash_input = input("Use Hashing (y/N): ").strip().upper()
        args.hash = hash_input in ['Y', 'YES']

    return args

def main():
    parser = argparse.ArgumentParser(description="Intention Repeater")
    parser.add_argument("--intent", help="Intention to repeat")
    parser.add_argument("--file", help="Path to the first file (use 'none' for no file)")
    parser.add_argument("--file2", help="Path to the second file (use 'none' for no file)")
    parser.add_argument("--freq", type=float, help="Frequency in Hz (0 = MAX)")
    parser.add_argument("--runtime", type=int, help="Total runtime in seconds (0 = Infinite)")
    parser.add_argument("--log", help="Path to save the log file (use 'none' for no log)")
    parser.add_argument("--hash", choices=['y', 'n'], help="Use hashing (y/n)")

    args = parser.parse_args()
    
    # Convert --hash argument to boolean
    if args.hash is not None:
        args.hash = args.hash.lower() == 'y'
    else:
        # If --hash was not provided, set it to None so that get_user_input will prompt for it
        args.hash = None

    args = get_user_input(args)

    intention = args.intent
    file1 = None if args.file is None or args.file.lower() == 'none' else args.file
    file2 = None if args.file2 is None or args.file2.lower() == 'none' else args.file2
    frequency = max(0, float(''.join(c for c in str(args.freq) if c.isdigit() or c == '.')))
    runtime = max(0, args.runtime)
    log_file = None if args.log is None or args.log.lower() == 'none' else args.log
    use_hashing = args.hash

    intention_repeater(intention, file1, file2, frequency, runtime, log_file, use_hashing)

if __name__ == "__main__":
    main()
