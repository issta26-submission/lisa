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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    const char *fruits_arr[] = { "apple", "banana", "cherry" };
    cJSON *fruits = cJSON_CreateStringArray(fruits_arr, 3);
    cJSON_AddItemToObject(root, "fruits", fruits);
    const char *raw_source = " { \"note\" : \"this is spaced\" } ";
    size_t raw_len = strlen(raw_source) + 1;
    char *raw_buf = (char *)cJSON_malloc(raw_len);
    memcpy(raw_buf, raw_source, raw_len);
    cJSON_Minify(raw_buf);
    cJSON *raw_item = cJSON_CreateRaw(raw_buf);
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate & Validate
    cJSON_bool has_fruits = cJSON_HasObjectItem(root, "fruits");
    cJSON_AddItemToObject(root, "has_fruits", cJSON_CreateBool(has_fruits));
    cJSON *got_greeting = cJSON_GetObjectItem(root, "greeting");
    const char *greet_text = cJSON_GetStringValue(got_greeting);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString(greet_text));
    cJSON *got_fruits = cJSON_GetObjectItem(root, "fruits");
    int fruits_count = cJSON_GetArraySize(got_fruits);
    cJSON_AddItemToObject(root, "fruits_count", cJSON_CreateNumber((double)fruits_count));

    // step 4: Cleanup
    cJSON_free(raw_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}