#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"temperature\": 23.5, \"sensor_id\": 7 }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *temp_item = cJSON_GetObjectItem(root, "temperature");
    cJSON *sensor_item = cJSON_GetObjectItem(root, "sensor_id");
    double temp_value = cJSON_GetNumberValue(temp_item);
    double sensor_value_before = cJSON_GetNumberValue(sensor_item);
    double readings_values[3];
    readings_values[0] = temp_value;
    readings_values[1] = temp_value + 1.0;
    readings_values[2] = temp_value + 2.0;
    cJSON *readings_array = cJSON_CreateDoubleArray(readings_values, 3);
    cJSON_AddItemToObject(root, "readings", readings_array);

    // step 3: Operate
    cJSON *new_sensor_item = cJSON_CreateNumber(sensor_value_before + 100.0);
    cJSON_ReplaceItemViaPointer(root, sensor_item, new_sensor_item);
    cJSON *sensor_item_after = cJSON_GetObjectItem(root, "sensor_id");
    double sensor_value_after = cJSON_GetNumberValue(sensor_item_after);

    // step 4: Validate & Cleanup
    (void)temp_value;
    (void)sensor_value_before;
    (void)sensor_value_after;
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}