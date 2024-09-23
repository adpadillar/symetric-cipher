import tkinter as tk
from tkinter import messagebox
import subprocess
import os
import base64

class EncryptionGUI:
    def __init__(self, master):
        self.master = master
        master.title("Encryption/Decryption Program")

        # Input fields
        self.key_label = tk.Label(master, text="Key:")
        self.key_label.grid(row=0, column=0, sticky="e")
        self.key_entry = tk.Entry(master)
        self.key_entry.grid(row=0, column=1)

        self.input_label = tk.Label(master, text="Input:")
        self.input_label.grid(row=1, column=0, sticky="e")
        self.input_text = tk.Text(master, height=10, width=50)
        self.input_text.grid(row=1, column=1)

        # Mode selection
        self.mode_var = tk.StringVar(value="encrypt")
        self.encrypt_radio = tk.Radiobutton(master, text="Encrypt", variable=self.mode_var, value="encrypt")
        self.encrypt_radio.grid(row=2, column=0)
        self.decrypt_radio = tk.Radiobutton(master, text="Decrypt", variable=self.mode_var, value="decrypt")
        self.decrypt_radio.grid(row=2, column=1)

        # Process button
        self.process_button = tk.Button(master, text="Process", command=self.process)
        self.process_button.grid(row=3, column=0, columnspan=2)

        # Output
        self.output_label = tk.Label(master, text="Output:")
        self.output_label.grid(row=4, column=0, sticky="e")
        self.output_text = tk.Text(master, height=10, width=50)
        self.output_text.grid(row=4, column=1)

    def process(self):
        key = self.key_entry.get()
        input_text = self.input_text.get("1.0", tk.END).strip()
        mode = self.mode_var.get()

        if not key or not input_text:
            messagebox.showerror("Error", "Please provide both key and input text.")
            return

        # Create temporary key file
        with open("temp_key.txt", "w") as key_file:
            key_file.write(key)

        # Create temporary input file
        if mode == "decrypt":
            try:
                # Decode base64 input for decryption
                input_text = base64.b64decode(input_text)
            except:
                messagebox.showerror("Error", "Invalid base64 input for decryption.")
                return
        
        with open("temp_input.txt", "wb") as input_file:
            input_file.write(input_text.encode() if isinstance(input_text, str) else input_text)

        try:
            # Run the encryption program
            result = subprocess.run(
                ["./bin/encryption_program", mode, "temp_input.txt", "temp_key.txt"],
                capture_output=True,
                check=True
            )

            # Display the output
            self.output_text.delete(1.0, tk.END)
            if mode == "encrypt":
                # Encode output as base64 for encryption
                output = base64.b64encode(result.stdout).decode()
            else:
                # For decryption, assume the output is text
                output = result.stdout.decode()
            self.output_text.insert(tk.END, output)

        except subprocess.CalledProcessError as e:
            messagebox.showerror("Error", f"An error occurred: {e}")
        finally:
            # Clean up temporary files
            os.remove("temp_key.txt")
            os.remove("temp_input.txt")

root = tk.Tk()
gui = EncryptionGUI(root)
root.mainloop()
