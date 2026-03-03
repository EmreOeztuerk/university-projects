FROM python:3.9-slim

# Install required packages
RUN pip install pandas paho-mqtt

# Create app directory
WORKDIR /app

# Copy source code
COPY src/mqtt /app/mqtt
COPY src/calculation_service /app/calculation_service


# Set Python path
ENV PYTHONPATH=/app

# Run the application
CMD ["python", "calculation_service/run.py"]