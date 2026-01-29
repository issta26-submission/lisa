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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON *e_item = cJSON_AddNumberToObject(root, "e", 2.718281828459045);

    // step 2: Configure
    size_t buf_size = 256;
    char *scratch = (char *)cJSON_malloc(buf_size);
    memset(scratch, 0, buf_size);

    // step 3: Operate & Validate
    cJSON *retrieved_pi = cJSON_GetObjectItem(root, "pi");
    double pi_value = cJSON_GetNumberValue(retrieved_pi);
    cJSON *pi_scaled = cJSON_CreateNumber(pi_value * 2.0);
    cJSON_AddItemToObject(root, "pi_scaled", pi_scaled);
    char *printed = cJSON_PrintBuffered(root, 32, 1);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}