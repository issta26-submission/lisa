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
//<ID> 1381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_a = cJSON_AddNumberToObject(root, "alpha", 42.25);
    cJSON *num_b = cJSON_AddNumberToObject(root, "beta", -7.75);

    // step 2: Configure
    cJSON *fetched = cJSON_GetObjectItem(root, "alpha");
    double alpha_val = cJSON_GetNumberValue(fetched);
    cJSON *alpha_plus = cJSON_AddNumberToObject(root, "alpha_plus", alpha_val + 1.0);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, 256, 0);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItem(root, "alpha_plus");
    double got_val = cJSON_GetNumberValue(got);
    int digit_got = ((int)got_val % 10 + 10) % 10;
    int digit_alpha = ((int)alpha_val % 10 + 10) % 10;
    int printed_int = (int)printed;
    buffer[0] = (char)('0' + digit_got);
    buffer[1] = (char)('0' + digit_alpha);
    buffer[2] = (char)('0' + printed_int);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}