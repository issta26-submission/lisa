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
//<ID> 499
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
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON *retr_meta = NULL;
    cJSON *retr_list = NULL;
    cJSON *retr_second = NULL;
    const char *second_str = NULL;
    const char *title_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(meta, "list", arr);
    cJSON_AddItemToObject(meta, "enabled", true_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(root, "title", "rootTitle");

    // step 3: Operate / Validate
    retr_meta = cJSON_GetObjectItem(root, "meta");
    retr_list = cJSON_GetObjectItem(retr_meta, "list");
    retr_second = cJSON_GetArrayItem(retr_list, 1);
    second_str = cJSON_GetStringValue(retr_second);
    title_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));
    buffer[0] = second_str[0];
    buffer[1] = title_str[0];
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}