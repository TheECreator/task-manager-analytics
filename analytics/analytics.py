import sqlite3
import pandas as pd
import matplotlib.pyplot as plt

# Connect to SQLite database
conn = sqlite3.connect('database/tasks.db')
cursor = conn.cursor()

# Fetch tasks from database
cursor.execute("SELECT status, COUNT(*) FROM tasks GROUP BY status;")
data = cursor.fetchall()

# Debug: Print the data fetched
print("Data fetched from DB:")
print(data)

# Close the connection
conn.close()

# Convert to DataFrame
df = pd.DataFrame(data, columns=["Status", "Count"])

# Debug: Print the DataFrame
print("Data in DataFrame:")
print(df)

# Plotting
def plot_status_distribution():
    plt.figure(figsize=(6, 6))
    plt.pie(df['Count'], labels=df['Status'], autopct='%1.1f%%', startangle=140)
    plt.title("Task Status Distribution")
    plt.show()

# Plot the distribution
plot_status_distribution()
