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
//<ID> 1285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *child = cJSON_CreateObject();
    char *buffer = (char *)cJSON_malloc((size_t)16);
    memset(buffer, 0, (size_t)16);
    buffer[0] = 'b'; buffer[1] = 'u'; buffer[2] = 'f'; buffer[3] = 'f'; buffer[4] = 'e'; buffer[5] = 'r'; buffer[6] = 'e'; buffer[7] = 'd'; buffer[8] = '\0';
    cJSON *bufstr = cJSON_CreateString(buffer);
    cJSON *name = cJSON_CreateString("example");

    // step 2: Configure
    cJSON_AddItemToObject(child, "name", name);
    cJSON_AddItemToArray(items, child);
    cJSON *arr_ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToArray(items, arr_ref);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "buffer", bufstr);

    // step 3: Operate & Validate
    char *printed = cJSON_Print(root);
    cJSON *got_buf = cJSON_GetObjectItem(root, "buffer");
    char *got_str = cJSON_GetStringValue(got_buf);
    (void)got_str;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}