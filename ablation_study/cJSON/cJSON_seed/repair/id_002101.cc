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
//<ID> 2101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *numitem = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(meta, "version", numitem);
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(nested, "greet", greet);

    // step 2: Configure
    cJSON_SetNumberHelper(numitem, 3.14159);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(meta, "version");
    double detached_val = cJSON_GetNumberValue(detached);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : '\0';
    int moddigit = (((int)detached_val % 10) + 10) % 10;
    buffer[1] = (char)('0' + moddigit);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}