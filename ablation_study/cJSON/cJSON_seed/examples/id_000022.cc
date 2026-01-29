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
//<ID> 22
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
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", nullable);

    // step 2: Configure
    cJSON *answer = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(metadata, "answer", answer);
    const int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 3: Operate & Validate
    cJSON *fetched = cJSON_GetObjectItem(metadata, "answer");
    double val = cJSON_GetNumberValue(fetched);
    double doubled = val * 2.0;
    cJSON *double_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(root, "double_answer", double_item);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}