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
//<ID> 1270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(container, "items", array);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *s1 = cJSON_CreateString("hello");
    cJSON *n2 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddStringToObject(root, "title", "example");

    // step 3: Operate
    cJSON *second = cJSON_GetArrayItem(array, 1);
    cJSON_bool second_invalid = cJSON_IsInvalid(second);
    (void)second_invalid;
    char *printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}