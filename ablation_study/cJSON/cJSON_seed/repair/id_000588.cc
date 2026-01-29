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
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *item1 = cJSON_CreateNumber(10.0);
    cJSON *item2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(data, item1);
    cJSON_AddItemToArray(data, item2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count_ptr = cJSON_AddNumberToObject(meta, "count", 2.0);
    cJSON *avg_ptr = cJSON_AddNumberToObject(meta, "avg", 15.0);

    // step 2: Configure
    cJSON *new_item = cJSON_CreateNumber(15.0);
    cJSON_InsertItemInArray(data, 1, new_item);
    cJSON_AddNumberToObject(root, "insert_position", 1.0);

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *got_count = cJSON_GetObjectItem(got_meta, "count");
    cJSON *got_avg = cJSON_GetObjectItemCaseSensitive(got_meta, "avg");
    double count_val = cJSON_GetNumberValue(got_count);
    double avg_val = cJSON_GetNumberValue(got_avg);
    double derived = count_val + avg_val;
    cJSON_AddNumberToObject(root, "derived", derived);
    cJSON_AddNumberToObject(root, "has_meta", (double)has_meta);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}