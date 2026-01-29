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
//<ID> 1757
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
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddItemToObject(entry1, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(entry1, "value", cJSON_CreateNumber(1.23));
    cJSON_AddItemToArray(items, entry1);
    cJSON *entry2 = cJSON_CreateObject();
    cJSON_AddItemToObject(entry2, "name", cJSON_CreateString("beta"));
    cJSON_AddItemToObject(entry2, "value", cJSON_CreateNumber(4.56));
    cJSON_AddItemToArray(items, entry2);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateObject());

    // step 3: Operate and Validate
    char *printed_root = cJSON_Print(root);
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, copy, 1);
    char *printed_copy = cJSON_Print(copy);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed_root ? printed_root[0] : 'X';
    buffer[1] = printed_copy ? printed_copy[0] : 'Y';
    buffer[2] = (char)('0' + (int)equal);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_copy);
    cJSON_free(buffer);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}