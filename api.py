import subprocess

# The message to send to the C application
message = "CHART golfers IN scatter_plot FOR driving_distance VS score WHERE tournament = Masters"

# Call the C program using subprocess and send the message
process = subprocess.Popen(
    ['./main'],  # Path to your C executable
    stdin=subprocess.PIPE,  # Pipe for sending input
    stdout=subprocess.PIPE,  # Pipe for receiving output
    stderr=subprocess.PIPE,
    text=True  # Automatically handles text encoding/decoding
)

# Send the message to C program and get the result
output, errors = process.communicate(input=message)

# Print the response from the C program
print("Received from C app:", output)
