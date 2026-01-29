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
//<ID> 2037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_buf[] = " { \"greeting\" : \"hello world\" , \"unused\" : false } ";
    cJSON_Minify(json_buf);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "active");

    // step 2: Configure
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", array);
    cJSON *item0 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, item0);
    cJSON *item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, item1);

    // step 3: Operate and Validate
    cJSON *replacement = cJSON_CreateString("replaced_value");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "list", replacement);
    cJSON *obj_list = cJSON_GetObjectItem(root, "list");
    char *strval = cJSON_GetStringValue(obj_list);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = json_buf[0];
    buffer[1] = strval[0];
    buffer[2] = '\0';
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}