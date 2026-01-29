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
//<ID> 523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    int buf_len = 256;
    char *prealloc = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc, 0, (size_t)buf_len);
    char *printed = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("example_string");
    cJSON *s2 = NULL;
    cJSON_bool wrote_prealloc = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "first", s1);
    s2 = cJSON_AddStringToObject(root, "second", "second_value");

    // step 3: Core operations / Validate
    wrote_prealloc = cJSON_PrintPreallocated(root, prealloc, buf_len, 1);
    printed = cJSON_Print(root);
    prealloc[0] = printed ? printed[0] : 'X';
    prealloc[1] = (char)('0' + (char)(wrote_prealloc ? 1 : 0));
    prealloc[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}