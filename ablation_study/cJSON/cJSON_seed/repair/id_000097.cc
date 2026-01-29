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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *title = cJSON_CreateString("original");
    cJSON_AddItemToObject(meta, "title", title);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *v1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, v1);
    cJSON *v2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, v2);

    // step 2: Configure
    char *prev_title = cJSON_SetValuestring(title, "updated");
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_backup", meta_dup);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_title = cJSON_GetObjectItem(got_meta, "title");
    char *title_str = cJSON_GetStringValue(got_title);
    int values_count = cJSON_GetArraySize(values);
    cJSON_AddNumberToObject(root, "values_count", (double)values_count);
    cJSON_AddNumberToObject(root, "title_first_char", (double)(unsigned char)title_str[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)prev_title;
    return 66; // API sequence test completed successfully
}