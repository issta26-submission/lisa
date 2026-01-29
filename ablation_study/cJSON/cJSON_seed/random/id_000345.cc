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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *child = (cJSON *)0;
    cJSON *ref_array = (cJSON *)0;
    cJSON *added_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *printed = (char *)0;
    char *first_str = (char *)0;
    cJSON_bool add_ok = (cJSON_bool)0;

    // step 2: Initialize
    child = cJSON_CreateString("alpha");
    ref_array = cJSON_CreateArrayReference(child);

    // step 3: Configure
    added_item = cJSON_CreateString("beta");
    add_ok = cJSON_AddItemToArray(ref_array, added_item);

    // step 4: Operate
    first_elem = cJSON_GetArrayItem(ref_array, 0);
    printed = cJSON_PrintUnformatted(ref_array);

    // step 5: Validate
    first_str = cJSON_GetStringValue(first_elem);
    (void)first_str;
    (void)add_ok;
    (void)printed;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(ref_array);

    // API sequence test completed successfully
    return 66;
}