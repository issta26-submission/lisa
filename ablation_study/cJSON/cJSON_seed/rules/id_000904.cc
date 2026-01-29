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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_literal = "{\"flags\": [true, false, true]}";
    cJSON *root = NULL;
    cJSON *flags = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON_bool is0 = 0;
    cJSON_bool is1 = 0;
    cJSON_bool is2 = 0;
    int true_count = 0;
    int *count_ptr = NULL;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_literal);
    flags = cJSON_GetObjectItem(root, "flags");
    item0 = cJSON_GetArrayItem(flags, 0);
    item1 = cJSON_GetArrayItem(flags, 1);
    item2 = cJSON_GetArrayItem(flags, 2);
    is0 = cJSON_IsTrue(item0);
    is1 = cJSON_IsTrue(item1);
    is2 = cJSON_IsTrue(item2);
    true_count = (int)is0 + (int)is1 + (int)is2;
    count_ptr = (int *)cJSON_malloc(sizeof(int));
    *count_ptr = true_count;

    // step 3: Operate / Validate
    (void)cJSON_GetArrayItem(flags, 0);
    (void)count_ptr;
    (void)true_count;
    (void)is0;
    (void)is1;
    (void)is2;

    // step 4: Cleanup
    cJSON_free(count_ptr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}