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
//<ID> 498
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_item = cJSON_CreateString("first_value");
    cJSON *retr_meta = NULL;
    cJSON *retr_flag = NULL;
    cJSON *retr_list = NULL;
    cJSON *retr_first = NULL;
    const char *first_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(meta, "flag", true_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate / Validate
    retr_meta = cJSON_GetObjectItem(root, "meta");
    retr_flag = cJSON_GetObjectItem(retr_meta, "flag");
    retr_list = cJSON_GetObjectItem(root, "list");
    retr_first = cJSON_GetArrayItem(retr_list, 0);
    first_str = cJSON_GetStringValue(retr_first);
    buffer[0] = first_str[0];
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}