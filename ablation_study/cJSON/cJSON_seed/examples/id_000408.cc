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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"greeting\" : \" Hello, World \", \"count\" : 3 } ";
    cJSON_Minify(json_buf);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_buf, &parse_end, 1);
    cJSON *greet = cJSON_GetObjectItem(root, "greeting");
    const char *greet_str = cJSON_GetStringValue(greet);

    // step 2: Configure
    const char *names[] = { "Alice", "Bob", "Eve" };
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_data", raw_item);
    cJSON *names_copy = cJSON_Duplicate(names_arr, 1);
    cJSON_AddItemToObject(root, "names_copy", names_copy);

    // step 3: Operate & Validate
    int names_count = cJSON_GetArraySize(names_arr);
    cJSON *first_name_item = cJSON_GetArrayItem(names_arr, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    cJSON_bool raw_is = cJSON_IsRaw(raw_item);
    char status_buf[192];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "greet='%s'|first='%s'|count=%d|raw=%d", greet_str ? greet_str : "(null)", first_name ? first_name : "(null)", names_count, (int)raw_is);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_item = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_item);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}