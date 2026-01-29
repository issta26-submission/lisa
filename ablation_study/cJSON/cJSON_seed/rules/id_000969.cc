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
//<ID> 969
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nums = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *raw_json = "{\"source\":\"sensor\",\"ok\":true}";
    double extracted = 0.0;
    int intpart = 0;
    size_t scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nums = cJSON_AddArrayToObject(root, "numbers");
    n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(nums, n1);
    n2 = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "pi", n2);
    cJSON_AddRawToObject(root, "metadata", raw_json);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    extracted = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    intpart = (int)extracted;
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = (char)('0' + (intpart % 10));
    scratch[1] = '_';
    scratch[2] = (char)('0' + ((int)nums->type % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}