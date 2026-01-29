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
//<ID> 1382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_a = cJSON_AddNumberToObject(root, "a", 42.5);
    cJSON *num_b = cJSON_AddNumberToObject(root, "b", -3.25);

    // step 2: Configure
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buf_len, 1);

    // step 3: Operate and Validate
    cJSON *got_a = cJSON_GetObjectItem(root, "a");
    double val_a = cJSON_GetNumberValue(got_a);
    cJSON *got_b = cJSON_GetObjectItem(root, "b");
    double val_b = cJSON_GetNumberValue(got_b);
    buffer[0] = (char)('0' + ((int)val_a % 10));
    buffer[1] = (char)('0' + (((int)val_b % 10 + 10) % 10));
    buffer[2] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}