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
//<ID> 1387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON_AddNumberToObject(stats, "count", 42.0);
    cJSON_AddNumberToObject(root, "version", 1.5);

    // step 2: Configure
    cJSON *count_item = cJSON_GetObjectItem(stats, "count");
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    double count_val = cJSON_GetNumberValue(count_item);
    double version_val = cJSON_GetNumberValue(version_item);

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 1);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    char *s = (char *)scratch;
    s[0] = printed ? '1' : '0';
    s[1] = (char)('0' + ((int)count_val % 10));
    s[2] = '.';
    s[3] = (char)('0' + (int)version_val);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}