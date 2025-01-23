import subprocess



message = "CHART golfers IN scatter_plot FOR driving_distance VS score WHERE tournament = Masters"

process = subprocess.Popen(
    ['./main'],  # Path to C executable
    stdin=subprocess.PIPE,  # Pipe for sending input
    stdout=subprocess.PIPE,  # Pipe for receiving output
    stderr=subprocess.PIPE,
    text=True
)

# Send the message to C intepreter
output, errors = process.communicate(input=message)

# Print response
print("Received from C app:", output)
