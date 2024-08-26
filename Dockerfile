# Use Python slim image to reduce size
FROM python:3.10.3-slim

# Set environment variables
ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1

# Set the working directory
WORKDIR /moyan

# Copy only the requirements file initially
COPY . .

# Install dependencies
RUN pip install --no-cache-dir --upgrade -r requirements.txt

# Create a non-root user for running the application
RUN adduser --system --group moyan \
    && chown -R moyan:moyan /moyan

# Switch to the non-root user
USER moyan

# Expose the port that the app runs on
EXPOSE 8896

# Command to run the application
CMD ["python", "main.py"]
