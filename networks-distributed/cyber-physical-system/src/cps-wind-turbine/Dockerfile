FROM python:3.9-slim

# Install required packages
RUN pip install pandas paho-mqtt

# Create app directory
WORKDIR /app

# Copy source code
COPY src/wind_turbine /app/wind_turbine
COPY src/mqtt /app/mqtt

# Copy wind energy data
COPY wind_energy_output.csv /data/wind_energy_output.csv

# Set Python path
ENV PYTHONPATH=/app

# Run the application
CMD ["python", "wind_turbine/run.py"] 