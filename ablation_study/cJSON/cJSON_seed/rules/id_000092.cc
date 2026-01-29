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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj2 = cJSON_CreateObject();
    cJSON *raw1 = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *raw2 = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *num_copy = cJSON_CreateNumber(3.14);
    char *printed_root = NULL;
    char *printed_obj2 = NULL;
    cJSON_bool equal = 0;
    double pi_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "raw", raw1);
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(obj2, "raw", raw2);
    cJSON_AddItemToObject(obj2, "pi", num_copy);

    // step 3: Operate / Validate
    equal = cJSON_Compare(root, obj2, 1);
    printed_root = cJSON_PrintUnformatted(root);
    printed_obj2 = cJSON_PrintUnformatted(obj2);
    pi_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    cJSON_AddNumberToObject(root, "pi_times_two", pi_val * 2.0);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_obj2);
    cJSON_Delete(root);
    cJSON_Delete(obj2);

    // API sequence test completed successfully
    return 66;
}