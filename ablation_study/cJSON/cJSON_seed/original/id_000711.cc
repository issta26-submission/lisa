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
//<ID> 711
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"key\": [1,2,3]}";
    size_t json_len = strlen(json_text);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    int nums[] = {10, 20, 30, 40};
    cJSON *int_arr = cJSON_CreateIntArray(nums, 4);
    cJSON *label = cJSON_CreateString("ints_label");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "ints", int_arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    int ints_size = cJSON_GetArraySize(int_arr);
    char count_buf[32];
    memset(count_buf, 0, sizeof(count_buf));
    sprintf(count_buf, "%d", ints_size);
    cJSON *count_str = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(root, "ints_count_str", count_str);

    // step 3: Operate
    const char *letters_json = "[\"a\",\"b\",\"c\"]";
    cJSON *letters = cJSON_ParseWithLengthOpts(letters_json, strlen(letters_json), NULL, 1);
    cJSON_AddItemToObject(root, "letters", letters);

    // step 4: Validate & Cleanup
    cJSON *inner = cJSON_GetObjectItem(parsed, "key");
    int inner_size = cJSON_GetArraySize(inner);
    cJSON_AddNumberToObject(root, "parsed_key_size", (double)inner_size);
    cJSON_AddNumberToObject(root, "ints_size", (double)ints_size);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}