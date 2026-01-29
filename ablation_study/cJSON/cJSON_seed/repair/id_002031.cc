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
//<ID> 2031
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *elem1 = cJSON_CreateString("one");
    cJSON *elem2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(array, elem1);
    cJSON_AddItemToArray(array, elem2);
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("active_replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "active", replacement);

    // step 3: Operate and Validate
    char json_txt[] = " { \"x\" :  1 , \"y\": 2 } ";
    cJSON_Minify(json_txt);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    const char *active_str = cJSON_GetStringValue(active_item);
    buffer[1] = active_str ? active_str[0] : '\0';
    buffer[2] = json_txt[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}