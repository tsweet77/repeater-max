import speech_recognition as sr
import pyttsx3
import subprocess

def speak(text):
    engine = pyttsx3.init()
    engine.say(text)
    engine.runAndWait()

def listen_for_command():
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening for command 'run repeater'...")
        audio = r.listen(source)

    try:
        command = r.recognize_google(audio).lower()
        print(f"Command: {command}")
        return command
    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print(f"Could not request results from Google Speech Recognition service; {e}")

    return ""

def get_intention():
    speak("What is your intention?")
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print("Listening for intention...")
        audio = r.listen(source, phrase_time_limit=2)

    try:
        intention = r.recognize_google(audio)
        print(f"Intention: {intention}")
        return intention
    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print(f"Could not request results from Google Speech Recognition service; {e}")

    return ""

def get_yes_no_response(question):
    speak(question)
    r = sr.Recognizer()
    with sr.Microphone() as source:
        print(f"Listening for response to: {question}")
        audio = r.listen(source, phrase_time_limit=2)

    try:
        response = r.recognize_google(audio).lower()
        print(f"Response: {response}")
        return "yes" in response
    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print(f"Could not request results from Google Speech Recognition service; {e}")

    return False

def main():
    while True:
        command = listen_for_command()

        if "run repeater intention" in command:
            intention = command.split("run repeater intention", 1)[1].strip()
            if not intention:
                intention = get_intention()
        elif "run repeater" in command:
            intention = get_intention()
        else:
            continue

        compress = "y" if get_yes_no_response("Would you like compression?") else "n"
        hashing = "y" if get_yes_no_response("Would you like hashing?") else "n"

        speak("Launching intention repeater")
        subprocess.run(["intention_repeater_max.exe", "--intent", intention, "--compress", compress, "--hashing", hashing, "--dur", "00:10:00"])

if __name__ == "__main__":
    main()