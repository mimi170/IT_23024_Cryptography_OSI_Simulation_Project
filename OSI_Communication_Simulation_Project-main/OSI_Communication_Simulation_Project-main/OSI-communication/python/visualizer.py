
import tkinter as tk
from tkinter import messagebox
import time

LAYERS = [
    "Application",
    "Presentation",
    "Session",
    "Transport",
    "Network",
    "Data Link",
    "Physical"
]

TAGS = ["APP", "PRS", "SES", "TCP", "IP", "MAC", "PHY"]

class OSIVisualizer:
    def __init__(self, root):
        self.root = root
        self.root.title("OSI Communication Simulation")
        self.root.geometry("1000x750")
        self.root.configure(bg="#101827")

        tk.Label(
            root, text="OSI MODEL COMMUNICATION SIMULATION",
            font=("Arial", 19, "bold"),
            bg="#101827", fg="white"
        ).pack(pady=15)

        top = tk.Frame(root, bg="#101827")
        top.pack(pady=5)

        tk.Label(
            top, text="Enter Message:",
            font=("Arial", 12), bg="#101827", fg="white"
        ).pack(side="left", padx=5)

        self.entry = tk.Entry(top, width=35, font=("Arial", 12))
        self.entry.insert(0, "Hello OSI Model")
        self.entry.pack(side="left", padx=5)

        tk.Button(
            top, text="Start Simulation",
            command=self.start,
            bg="#16A34A", fg="white",
            font=("Arial", 11, "bold")
        ).pack(side="left", padx=10)

        self.canvas = tk.Canvas(
            root, width=900, height=490,
            bg="#172235", highlightthickness=0
        )
        self.canvas.pack(pady=15)

        self.progress = tk.Label(
            root, text="Progress: Waiting...",
            font=("Arial", 13, "bold"),
            bg="#101827", fg="#FACC15"
        )
        self.progress.pack(pady=5)

        self.status = tk.Label(
            root, text="Ready to start communication",
            font=("Arial", 11),
            bg="#101827", fg="white"
        )
        self.status.pack(pady=5)

        self.draw_layers()

    def draw_layers(self):
        self.canvas.delete("all")
        self.positions = []

        for i, layer in enumerate(LAYERS):
            x = 100 if i % 2 == 0 else 550
            y = 25 + i * 65

            self.positions.append((x, y))

            self.canvas.create_rectangle(
                x, y, x + 250, y + 42,
                fill="#263B59", outline="#38BDF8", width=2,
                tags=f"layer{i}"
            )

            self.canvas.create_text(
                x + 125, y + 21,
                text=f"{i+1}. {layer}",
                fill="white", font=("Arial", 12, "bold")
            )

        for i in range(6):
            x1, y1 = self.positions[i]
            x2, y2 = self.positions[i + 1]

            self.canvas.create_line(
                x1 + 125, y1 + 42,
                x2 + 125, y2,
                fill="#94A3B8", width=2,
                arrow=tk.LAST
            )

        self.canvas.create_text(
            450, 475,
            text="ZIG-ZAG OSI DATA FLOW",
            fill="#FACC15", font=("Arial", 12, "bold")
        )

    def start(self):
        message = self.entry.get().strip()

        if not message:
            messagebox.showwarning("Warning", "Enter a message first!")
            return

        self.draw_layers()
        self.status.config(text="Starting Encapsulation...")
        self.progress.config(text="Progress: 0%")

        packet = message

        for i in range(7):
            packet = f"{TAGS[i]}{{{packet}}}"

            x, y = self.positions[i]
            self.canvas.create_rectangle(
                x, y, x + 250, y + 42,
                fill="#15803D", outline="#4ADE80", width=3
            )

            self.canvas.create_text(
                x + 125, y + 21,
                text=f"{i+1}. {LAYERS[i]}",
                fill="white", font=("Arial", 12, "bold")
            )

            print(f"[ENCAPSULATION] {LAYERS[i]}: {packet}")

            if (i + 1) % 2 == 0 or i == 6:
                percent = int((i + 1) / 7 * 100)
                print(f"Encapsulation Progress: {percent}%")
                self.progress.config(
                    text=f"Encapsulation Progress: {percent}%"
                )
                self.root.update()
                time.sleep(0.5)

        print("\n=== TRANSMISSION COMPLETED ===")
        self.status.config(text="Data transmitted through all 7 layers!")

        self.root.update()
        time.sleep(1)

        self.status.config(text="Starting Decapsulation...")

        for i in range(6, -1, -1):
            prefix = TAGS[i] + "{"

            if not packet.startswith(prefix) or not packet.endswith("}"):
                messagebox.showerror("Error", "Invalid packet format!")
                return

            packet = packet[len(prefix):-1]

            x, y = self.positions[i]
            self.canvas.create_rectangle(
                x, y, x + 250, y + 42,
                fill="#1D4ED8", outline="#60A5FA", width=3
            )

            self.canvas.create_text(
                x + 125, y + 21,
                text=f"{i+1}. {LAYERS[i]}",
                fill="white", font=("Arial", 12, "bold")
            )

            print(f"[DECAPSULATION] {LAYERS[i]}: {packet}")

            completed = 7 - i

            if completed % 2 == 0 or i == 0:
                percent = int(completed / 7 * 100)
                print(f"Decapsulation Progress: {percent}%")
                self.progress.config(
                    text=f"Decapsulation Progress: {percent}%"
                )
                self.root.update()
                time.sleep(0.5)

        self.status.config(
            text=f"SUCCESS! Final Message Delivered: {packet}"
        )

        self.progress.config(text="Communication Progress: 100%")
        print("\n=== FINAL MESSAGE DELIVERED ===")
        print("Final Message:", packet)

        messagebox.showinfo(
            "Success",
            f"Message delivered successfully!\n\n{packet}"
        )


if __name__ == "__main__":
    window = tk.Tk()
    app = OSIVisualizer(window)
    window.mainloop()
