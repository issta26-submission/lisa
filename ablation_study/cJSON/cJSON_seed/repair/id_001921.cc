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
//<ID> 1921
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
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, true_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddNullToObject(child, "maybe");
    cJSON_AddItemToArray(array, child);

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    char smallbuf[2];
    smallbuf[0] = buffer[0];
    smallbuf[1] = '\0';
    cJSON *derived = cJSON_CreateString(smallbuf);
    cJSON_AddItemToObject(root, "derived", derived);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}