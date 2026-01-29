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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json1[] = "{\"value\":3.14,\"name\":\"test\"}";
    const char json2[] = "{\"value\":3.14,\"name\":\"test\"}";
    const char *end1 = (const char *)0;
    const char *end2 = (const char *)0;
    cJSON *root1 = (cJSON *)0;
    cJSON *root2 = (cJSON *)0;
    cJSON *value_item = (cJSON *)0;
    double extracted = 0.0;
    cJSON_bool equal = 0;

    // step 2: Setup / Parse
    root1 = cJSON_ParseWithLengthOpts(json1, (size_t)(sizeof(json1) - 1), &end1, 1);
    root2 = cJSON_ParseWithLengthOpts(json2, (size_t)(sizeof(json2) - 1), &end2, 1);

    // step 3: Operate / Validate
    value_item = cJSON_GetObjectItem(root1, "value");
    extracted = cJSON_GetNumberValue(value_item);
    equal = cJSON_Compare(root1, root2, 1);

    // step 4: Cleanup
    cJSON_Delete(root2);
    cJSON_Delete(root1);

    // API sequence test completed successfully
    return 66;
}