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
//<ID> 1781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON_AddNumberToObject(root, "threshold", 42.0);
    cJSON_AddNumberToObject(metadata, "version", 3.14);
    cJSON *author_item_created = cJSON_CreateString("tester");
    cJSON_AddItemToObject(metadata, "author", author_item_created);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(80);
    memset(buffer, 0, 80);

    // step 3: Operate and Validate
    cJSON *author_item = cJSON_GetObjectItem(metadata, "author");
    const char *author_str = cJSON_GetStringValue(author_item);
    cJSON *version_item = cJSON_GetObjectItem(metadata, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = author_str ? author_str[0] : 'Z';
    buffer[2] = (char)('0' + ((int)version_val % 10));
    cJSON_DeleteItemFromObject(root, "threshold");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}