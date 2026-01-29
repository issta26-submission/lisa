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
//<ID> 1841
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(payload, "list", list);
    cJSON_AddItemToArray(list, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(list, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(list, cJSON_CreateString("hello"));

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "author", cJSON_CreateString("tester"));
    cJSON *detached = cJSON_DetachItemFromArray(list, 1);
    cJSON_AddItemToObject(meta, "detached", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int size_list = cJSON_GetArraySize(list);
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_detached = cJSON_GetObjectItem(got_meta, "detached");
    double detached_val = cJSON_GetNumberValue(got_detached);
    cJSON *got_author = cJSON_GetObjectItem(got_meta, "author");
    const char *author = cJSON_GetStringValue(got_author);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = author ? author[0] : 'A';
    buffer[2] = (char)('0' + (size_list % 10));
    buffer[3] = (char)('0' + (((int)detached_val) % 10));
    (void)buffer; (void)author; (void)detached_val; (void)size_list; (void)got_meta;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}