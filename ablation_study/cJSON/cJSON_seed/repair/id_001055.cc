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
//<ID> 1055
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
    cJSON *title = cJSON_AddStringToObject(root, "title", "sample");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *elem1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, elem1);
    cJSON *elem2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(items, elem2);
    cJSON_AddStringToObject(meta, "owner", "fuzzer");

    // step 3: Operate and Validate
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_AddStringToObject(copy, "copied_from", "root");
    char *printed_root = cJSON_Print(root);
    char *printed_copy = cJSON_Print(copy);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    ((char *)buffer)[0] = printed_root ? printed_root[0] : '\0';
    ((char *)buffer)[1] = printed_copy ? printed_copy[0] : '\0';
    cJSON_free(buffer);
    cJSON_free(printed_root);
    cJSON_free(printed_copy);

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}