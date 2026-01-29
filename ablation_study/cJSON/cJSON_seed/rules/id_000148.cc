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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char input[] = "{\"alpha\":3.14,\"beta\":2}";
    size_t input_len = sizeof(input) - 1;
    const char * parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(input, input_len, &parse_end, 1);
    cJSON *alpha_item = NULL;
    cJSON *beta_item = NULL;
    double alpha_val = 0.0;
    double beta_val = 0.0;
    cJSON *constructed = NULL;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    alpha_item = cJSON_GetObjectItem(root, "alpha");
    beta_item = cJSON_GetObjectItem(root, "beta");
    alpha_val = cJSON_GetNumberValue(alpha_item);
    beta_val = cJSON_GetNumberValue(beta_item);

    // step 3: Operate / Validate
    constructed = cJSON_CreateObject();
    cJSON_AddNumberToObject(constructed, "alpha", alpha_val);
    cJSON_AddNumberToObject(constructed, "beta", beta_val);
    equal = cJSON_Compare(root, constructed, 1);

    // step 4: Cleanup
    cJSON_Delete(constructed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}