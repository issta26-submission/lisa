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
//<ID> 1024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON_AddNumberToObject(root, "version", 1.23);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    char *buf = (char *)cJSON_malloc(16);
    memset(buf, 0, 16);
    buf[0] = 'b'; buf[1] = 'u'; buf[2] = 'f'; buf[3] = 'f'; buf[4] = 'e'; buf[5] = 'r'; buf[6] = '\0';
    cJSON_AddStringToObject(meta, "note", buf);
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    cJSON_SetNumberHelper(version_item, 2.46);
    cJSON *numbers_item = cJSON_GetObjectItem(root, "numbers");
    cJSON_bool is_array = cJSON_IsArray(numbers_item);
    cJSON_AddBoolToObject(root, "numbers_is_array", is_array);

    // step 3: Operate and Validate
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_Delete(copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buf);

    // API sequence test completed successfully
    return 66;
}