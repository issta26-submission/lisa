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
//<ID> 1771
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *entries = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "entries", entries);
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddItemToArray(entries, entry1);
    cJSON *name1 = cJSON_CreateString("sensor-1");
    cJSON_AddItemToObject(entry1, "name", name1);
    cJSON *value1 = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(entry1, "value", value1);
    cJSON *entry2 = cJSON_CreateObject();
    cJSON_AddItemToArray(entries, entry2);
    cJSON_AddStringToObject(entry2, "name", "sensor-2");
    cJSON_AddNumberToObject(entry2, "value", 13.0);

    // step 2: Configure
    cJSON_AddStringToObject(root, "app", "telemetry");
    cJSON_AddStringToObject(root, "version", cJSON_Version());
    double adjusted_old = cJSON_SetNumberHelper(value1, 45.0);

    // step 3: Operate and Validate
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_entries = cJSON_GetObjectItem(got_settings, "entries");
    cJSON *first_entry = cJSON_GetArrayItem(got_entries, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_entry, "name");
    cJSON *value_item = cJSON_GetObjectItem(first_entry, "value");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    char *name_str = cJSON_GetStringValue(name_item);
    double value_num = cJSON_GetNumberValue(value_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)name_is_string);
    buffer[2] = name_str ? name_str[0] : 'Z';
    buffer[3] = (char)('0' + ((int)value_num % 10));
    buffer[4] = (char)('0' + ((int)adjusted_old % 10));
    (void)buffer;
    (void)name_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}