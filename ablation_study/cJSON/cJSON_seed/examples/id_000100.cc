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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "strings", arr);

    // step 2: Configure
    char *buf = (char *)cJSON_malloc(16);
    buf[0] = ' ';
    buf[1] = '{';
    buf[2] = ' ';
    buf[3] = '"';
    buf[4] = 'k';
    buf[5] = '"';
    buf[6] = ' ';
    buf[7] = ':';
    buf[8] = ' ';
    buf[9] = '"';
    buf[10] = 'v';
    buf[11] = '"';
    buf[12] = ' ';
    buf[13] = '}';
    buf[14] = ' ';
    buf[15] = '\0';
    cJSON_Minify(buf);
    cJSON *minified = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "minified", minified);

    // step 3: Operate & Validate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_text = cJSON_GetStringValue(first_item);
    cJSON *label = cJSON_CreateString(first_text ? first_text : "");
    cJSON_AddItemToObject(root, "first", label);
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON *has_flag = cJSON_CreateNumber((double)has_strings);
    cJSON_AddItemToObject(root, "has_strings", has_flag);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}