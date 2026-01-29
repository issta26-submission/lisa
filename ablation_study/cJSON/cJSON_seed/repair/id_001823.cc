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
//<ID> 1823
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(child, "count", 7.0);
    cJSON *nameItem = cJSON_CreateString("delta");
    cJSON_AddItemToObject(child, "name", nameItem);
    const char *ver = cJSON_Version();

    // step 2: Operate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *childRef = cJSON_GetObjectItem(root, "child");
    double count = cJSON_GetNumberValue(cJSON_GetObjectItem(childRef, "count"));
    const char *name = cJSON_GetStringValue(cJSON_GetObjectItem(childRef, "name"));
    buffer[0] = ver ? ver[0] : 'V';
    buffer[1] = (char)('0' + (((int)count) % 10));
    buffer[2] = name ? name[0] : 'N';
    buffer[3] = printed ? printed[0] : 'P';

    // step 3: Validate
    cJSON *lookup = cJSON_GetObjectItem(root, "child");
    double counted = cJSON_GetNumberValue(cJSON_GetObjectItem(lookup, "count"));
    (void)counted;
    (void)buffer;
    (void)ver;
    (void)name;
    (void)printed;
    (void)childRef;
    (void)lookup;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}