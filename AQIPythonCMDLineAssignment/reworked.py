# Taras Ermolenko
# taras_ermolenko@sfu.ca

import os
import sys
import requests
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

API_TOKEN = os.getenv('API_TOKEN') 
if not API_TOKEN:
    raise ValueError("API token not found. Need to set the API_TOKEN environment variable.")


def get_pm25_data(uid):
    """Gets the PM2.5 data for a given station UID."""
    url = f'https://api.waqi.info/feed/@{uid}/?token={API_TOKEN}'  # Format the request URL 
    try:
        response = requests.get(url)  # Make the HTTP GET request to the API
        response.raise_for_status()  # Raise an exception for HTTP errors
        return response.json(), None  # Return the JSON response and None for error
    except requests.exceptions.RequestException as e:
        return None, e  # In case of request failure, return None and the error


def get_station_ids(lat1, long1, lat2, long2):
    """Gets station IDs within specified geographic bounds."""
    latlng = f"{lat1},{long1},{lat2},{long2}"  # Combine coordinates into a single string
    url = f'https://api.waqi.info/map/bounds?token={API_TOKEN}&latlng={latlng}'  # Format request URL 
    try:
        response = requests.get(url)  # Make the HTTP GET request to the API
        response.raise_for_status()  # Check for HTTP errors
        return [station_data['uid'] for station_data in response.json()['data']], None  # Extract UIDs from response 
    except requests.exceptions.RequestException as e:
        return [], e  # Return empty list and error in case of failure


def station_sample(uid_list):
    """Gets the PM2.5 data for all stations in uid_list in parallel."""
    pm25_dict = {}  # Initialize dictionary to hold PM2.5 data
    with ThreadPoolExecutor(max_workers=20) as executor:  # Create a ThreadPoolExecutor as a context manager
        # Submit all API calls at once and map future to UID for identification
        future_to_uid = {executor.submit(get_pm25_data, uid): uid for uid in uid_list}
        for future in as_completed(future_to_uid):  # Iterate over futures as they complete
            uid = future_to_uid[future]  # Retrieve the UID associated with this future
            try:
                pm25_data, error = future.result()  # Get the result of the future, which is either data or an error
                if pm25_data and 'data' in pm25_data and 'iaqi' in pm25_data['data'] and 'pm25' in pm25_data['data']['iaqi']:
                    # Process and store the PM2.5 data if available
                    pm25_value = pm25_data['data']['iaqi']['pm25']['v']
                    station_name = pm25_data['data']['city']['name']
                    pm25_dict.setdefault(station_name, []).append(pm25_value)
                else:
                    print(f"PM2.5 data not available or error for station with UID {uid}: {error}")
            except Exception as exc:
                print(f"Error fetching data for UID {uid}: {exc}")
    return pm25_dict  # Return the dictionary containing PM2.5 data


def print_sampled_values_for_each_station(pm25_dict):
    """Prints the PM2.5 sampled values for each station."""
    for station_name, values in pm25_dict.items():
        print(f"{station_name}: Sampled PM2.5 values: {', '.join(map(str, values))}")


def print_average_for_each_station(pm25_dict):
    """Calculates and prints the average PM2.5 value for each station."""
    print("\nAverage PM2.5 Values for Each Station:")
    for station_name, values in pm25_dict.items():
        if values:
            average_value = sum(values) / len(values)
            print(f"{station_name}: Average PM2.5 value: {average_value:.2f}")
        else:
            print(f"{station_name}: No PM2.5 data available.")
            

def main(latitude1, longitude1, latitude2, longitude2, sample_period, sample_rate):
    """Main function that does the data fetching and processing."""
    uid_list, error = get_station_ids(latitude1, longitude1, latitude2, longitude2)  # Get the list of station UIDs
    if error:
        print(f"Error fetching station IDs: {error}")
        return

    pm25_dict = {}  # Initialize pm25_dict outside the loop to accumulate data
    total_samples = sample_period * sample_rate  # Calculate the total number of samples to be taken

    for _ in range(total_samples):
        # Fetch PM2.5 data in parallel and update pm25_dict with new data
        new_data = station_sample(uid_list)  
        for station, values in new_data.items():
            if station in pm25_dict:
                pm25_dict[station].extend(values)  # Add new values to existing list
            else:
                pm25_dict[station] = values  # Initialize list with new values for the station

        if _ < total_samples - 1:  # Avoid sleeping after the last sample
            time.sleep(60 / sample_rate)  # Sleep to achieve the desired sample rate per minute

    # After all data collection, print sampled values for each station
    print_sampled_values_for_each_station(pm25_dict)
    # Then, print the average PM2.5 value for each station
    print_average_for_each_station(pm25_dict)


if __name__ == "__main__":
    # Command line argument parsing
    if len(sys.argv) in [5, 7]:
        latitude_1, longitude_1, latitude_2, longitude_2 = map(float, sys.argv[1:5])
        sample_period, sample_rate = (5, 1) if len(sys.argv) == 5 else map(int, sys.argv[5:])
        main(latitude_1, longitude_1, latitude_2, longitude_2, sample_period, sample_rate)
    else:
        print("Usage: python script.py <latitude_1> <longitude_1> <latitude_2> <longitude_2> [<minutes> <rate>]")
